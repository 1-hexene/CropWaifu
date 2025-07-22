#include <control/control_tasks.h>

QueueHandle_t ctrlCmdQueue;
extern CropWaifuSensors cropWaifuSensors; // From sensor_tasks.cpp
extern SemaphoreHandle_t cropWaifuSensorsMutex; // From sensor_tasks.cpp

uint8_t fanPWM = 0;
uint8_t ledPWM = 0;

uint16_t targetLightIntensity = 0; // 目标光照强度
float targetTemperature = 0.0f; // 目标温度
bool enablePIDControl = false;


canwaifu_status control_init(void) {
    pinMode(PIN_FAN_CON, OUTPUT); // 设置风扇控制引脚为输出
    pinMode(PIN_LIGHT_CON, OUTPUT); // 设置LED控制引脚为输出
    ledcAttachPin(PIN_LIGHT_CON, 0); // 将风扇控制引脚连接到通道0
    ledcSetup(0, 500, 8); // 设置通道0的频率为500Hz，分辨率为8位
    ctrlCmdQueue = xQueueCreate(10, sizeof(ControlCommand*));
    Serial.println("[CTRL] Control command queue initialized.");
    return CANWAIFU_OK;
      // 初始化 PWM
  
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
                enablePIDControl = false; // 禁用PID控制
                Serial.printf("[CTRL] PID control disabled.");
                break;

            case CTRL_MODE_PID:
                targetLightIntensity = ctrlCmd->_lightIntensity; // 更新目标光照强度
                targetTemperature = ctrlCmd->_temperature; // 更新目标温度
                enablePIDControl = true; // 启用PID控制
                Serial.printf("[CTRL] PID control enabled.");
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
  int16_t error = 0;
  int16_t integral = 0; // 积分项

  while (true) {
    // Step 1: 获取当前光照值（线程安全）
    if (xSemaphoreTake(cropWaifuSensorsMutex, portMAX_DELAY) == pdTRUE) {
      currentLightIntensity = cropWaifuSensors.lightIntensity;
      cropWaifuSensors.ledPWM = ledPWM; // 更新LED PWM值到传感器对象
      xSemaphoreGive(cropWaifuSensorsMutex);
    }
    if (enablePIDControl) {
      error = (int16_t) (targetLightIntensity - currentLightIntensity);
      if (abs(error) < 30) {
        integral += error;
        integral = constrain(integral, -5000, 5000); // 积分限幅
      }

      int controlOutput = (int)((KP_LED * error + KI_LED * integral) / 1000);

      int tempPWM = ledPWM + controlOutput;

      ledPWM = constrain(tempPWM, PWM_MIN, PWM_MAX); // 限制PWM值在0-255之间

      ledcWrite(0, ledPWM); // 控制LED亮度
      // Serial.printf("[LED] LI: %d, PWM: %d\n",currentLightIntensity, ledPWM);
    }
    vTaskDelay(10 / portTICK_PERIOD_MS); // 每10ms调整一次LED亮度
  }
}

void fan_control_task(void *pvParameters) {
  float currentTemperature = 0.0f;
  int16_t error = 0;

  while (true) {
    // Step 1: 获取当前风扇PWM值（线程安全）
    if (xSemaphoreTake(cropWaifuSensorsMutex, portMAX_DELAY) == pdTRUE) {
      currentTemperature = cropWaifuSensors.temperature;
      cropWaifuSensors.fanPWM = fanPWM; // 更新风扇PWM值到传感器对象
      xSemaphoreGive(cropWaifuSensorsMutex);
    }
    if (enablePIDControl) { 
      error = (int16_t) ((currentTemperature - targetTemperature) * KP_FAN);
      error = error < PWM_MIN ? PWM_MIN : error;
      fanPWM = error > PWM_MAX ? PWM_MAX : (uint8_t) error;
      analogWrite(PIN_FAN_CON, fanPWM); // 控制风扇速度
      Serial.printf("[FAN] PWM: %d\n", fanPWM);
    }
    

    vTaskDelay(10 / portTICK_PERIOD_MS); // 每10ms调整一次风扇速度
  }
}