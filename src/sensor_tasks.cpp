#include <sensor/sensor_tasks.h>

/* Ambient Light Sensors */
BH1750Sensor als1(0x23);
BH1750Sensor als2(0x5C);

/* Temperature & */


CropWaifuSensors cropWaifuSensors = CropWaifuSensors();
extern canwaifu_status globalStatus; // From cropwaifu_daemon.cpp

canwaifu_status sensor_init() {
    // Initialize I2C for sensors
    Wire.begin(PIN_I2C_SDA, PIN_I2C_SCL);  // Wire.begin(sda, scl);

    if (als1.begin()) {
        Serial.println("[SENS] ALS 1 (0x23) initialized.");
    } else {
        Serial.println("[SENS] Failed to init ALS 1 (0x23).");
        return CANWAIFU_ERR;
    }

    if (als2.begin()) {
        Serial.println("[SENS] ALS2 (0x5C) initialized.");
    } else {
        Serial.println("Failed to init ALS 2 (0x5C).");
        return CANWAIFU_ERR;
    }

    return CANWAIFU_OK; // Return success status
}

void sensor_task(void *pvParameters) {
    Serial.println("[SENS] Sensor Task started");
    uint16_t lux1, lux2;

    while (true) {
        lux1 = als1.read_lux();
        lux2 = als2.read_lux();

        if (lux1 < 0 || lux2 < 0) {
            Serial.println("[SENS] Error reading lux values.");
            globalStatus = canwaifu_status(globalStatus | CANWAIFU_WARNING); // Update global status
            continue; // Skip this iteration if there's an error
        } else {
            globalStatus = canwaifu_status(globalStatus & ~CANWAIFU_WARNING); // Clear warning status
        }

        // Update CropWaifuSensors
        cropWaifuSensors.lightIntensity = (lux1 + lux2) / 2; // Average of both sensors

        vTaskDelay(1000 / portTICK_PERIOD_MS); // Update per 1000ms
    }
}
