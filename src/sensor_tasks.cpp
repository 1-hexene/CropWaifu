#include <sensor/sensor_tasks.h>

/* Ambient Light Sensors */
BH1750Sensor als1(0x23);
BH1750Sensor als2(0x5C);

/* Temperature & Humidity Sensor*/
SHT sht301(0x44);
SHT sht302(0x45);

CropWaifuSensors cropWaifuSensors = CropWaifuSensors();
extern canwaifu_status globalStatus; // From cropwaifu_daemon.cpp

canwaifu_status sensor_init() {
    // Initialize I2C for sensors
    Wire.begin(PIN_I2C_SDA, PIN_I2C_SCL);  // Wire.begin(sda, scl);

    // Initialize Ambient Light Sensors
    if (als1.begin()) {
        Serial.println("[SENS] ALS 1 (0x23) initialized.");
    } else {
        Serial.println("[SENS] Failed to init ALS #1 (0x23).");
        return CANWAIFU_ERR;
    }

    if (als2.begin()) {
        Serial.println("[SENS] ALS2 (0x5C) initialized.");
    } else {
        Serial.println("Failed to init ALS #2 (0x5C).");
        return CANWAIFU_ERR;
    }

    // Initialize SHT sensors
    if (sht301.begin()) {
        Serial.println("[SENS] SHT301 (0x44) initialized.");
    } else {
        Serial.println("[SENS] Failed to init SHT30 #1 (0x44).");
        return CANWAIFU_ERR;
    }

    if (sht302.begin()) {
        Serial.println("[SENS] SHT302 (0x45) initialized.");
    } else {
        Serial.println("[SENS] Failed to init SHT30 #2 (0x45).");
        return CANWAIFU_ERR;
    }

    return CANWAIFU_OK; // Return success status
}

void sensor_task(void *pvParameters) {
    Serial.println("[SENS] Sensor Task started");
    uint16_t lux1, lux2;
    float temp1, temp2, humidity1, humidity2;

    while (true) {
        lux1 = als1.read_lux();
        lux2 = als2.read_lux();

        if (lux1 < 0 || lux2 < 0) {
            Serial.println("[SENS] Error reading lux values. Resetting ALS...");
            globalStatus = canwaifu_status(globalStatus | CANWAIFU_WARNING); // Update global status
            digitalWrite(PIN_BH1750_RST, HIGH); // Reset BH1750 sensors
            vTaskDelay(1 / portTICK_PERIOD_MS); // Wait for 1ms
            digitalWrite(PIN_BH1750_RST, LOW); // Clear reset
            continue;
        } else {
            globalStatus = canwaifu_status(globalStatus & ~CANWAIFU_WARNING); // Clear warning status
        }

        sht301.requestData();
        sht302.requestData();
        vTaskDelay(1 / portTICK_PERIOD_MS); // Allow time for the sensors to process the request
        while (!sht301.dataReady() || !sht302.dataReady()) {
            vTaskDelay(1 / portTICK_PERIOD_MS); // Wait for data to be ready
        }
        sht301.readData();
        sht302.readData();
        temp1 = sht301.getTemperature();
        temp2 = sht302.getTemperature();
        humidity1 = sht301.getHumidity();
        humidity2 = sht302.getHumidity();

        cropWaifuSensors.lightIntensity = (lux1 + lux2) / 2; // Average of both sensors
        cropWaifuSensors.humidity = (humidity1 + humidity2) / 2.0f; // Average of both sensors
        cropWaifuSensors.temperature = (temp1 + temp2) / 2.0f; // Average of both
        Serial.printf("[SENS] Light Intensity: %d lux\n", cropWaifuSensors.lightIntensity);
        Serial.printf("[SENS] Temperature: %.2f °C\n", cropWaifuSensors.temperature);
        Serial.printf("[SENS] Humidity: %.2f %%\n", cropWaifuSensors.humidity);

        vTaskDelay(998 / portTICK_PERIOD_MS); // Update per 1000ms
    }
}
