#include <control/control_tasks.h>

QueueHandle_t ctrlCmdQueue;
extern CropWaifuSensors cropWaifuSensors; // From sensor_tasks.cpp
extern SemaphoreHandle_t cropWaifuSensorsMutex; // From sensor_tasks.cpp

uint8_t fanPWM = 0;
uint8_t ledPWM = 0;

uint16_t targetLightIntensity = 0; // 目标光照强度
float targetTemperature = 0.0f; // 目标温度


canwaifu_status control_init(void) {
    pinMode(PIN_FAN_CON, OUTPUT); // 设置风扇控制引脚为输出
    pinMode(PIN_LIGHT_CON, OUTPUT); // 设置LED控制引脚为输出
    ctrlCmdQueue = xQueueCreate(10, sizeof(ControlCommand*));
    Serial.println("[CTRL] Control command queue initialized.");
    return CANWAIFU_OK;
}

void control_task(void *pvParameters) {
    ControlCommand* ctrlCmd;
    
    while (true) {
        if (xQueueReceive(ctrlCmdQueue, &ctrlCmd, portMAX_DELAY) == pdPASS) {
            Serial.print("[CTRL] ");
            ctrlCmd->print();
            switch (ctrlCmd->_mode)
            {
            case CTRL_MODE_ABS:
                ledPWM = ctrlCmd->_led; // 更新LED PWM值
                fanPWM = ctrlCmd->_fan; // 更新风扇PWM值
                analogWrite(PIN_FAN_CON, ledPWM); // 控制风扇
                analogWrite(PIN_LIGHT_CON, fanPWM);
                break;

            case CTRL_MODE_PID:
                targetLightIntensity = ctrlCmd->_lightIntensity; // 更新目标光照强度
                targetTemperature = ctrlCmd->_temperature; // 更新目标温度
                break;
            
            default:
                break;
            }
            delete ctrlCmd;  // 用完记得释放内存
        }
        
    }
}

void led_control_task(void *pvParameters) {
    uint16_t currentLightIntensity = 0;
    while (true) {
        if (xSemaphoreTake(cropWaifuSensorsMutex, portMAX_DELAY) == pdTRUE) {
            currentLightIntensity = cropWaifuSensors.lightIntensity;
            xSemaphoreGive(cropWaifuSensorsMutex);
        } else {
            Serial.println("[CTRL] Failed to take sensor mutex for LED control.");
        }
        if (currentLightIntensity < targetLightIntensity) {
            ledPWM = map(currentLightIntensity, 0, targetLightIntensity, 0, 255);
        } else {
            ledPWM = 0; // 如果当前光照强度超过目标值，关闭LED
        }
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}
