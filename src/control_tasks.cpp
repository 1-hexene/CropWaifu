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
    ledcAttachPin(PIN_FAN_CON, PWM_CHANNEL_FAN); // 将风扇控制引脚连接到通道4
    ledcSetup(PWM_CHANNEL_FAN, 25000, 8); // 设置通道4的频率为25kHz，分辨率为8位

    pinMode(PIN_LIGHT_CON, OUTPUT); // 设置LED控制引脚为输出
    ledcAttachPin(PIN_LIGHT_CON, PWM_CHANNEL_LIGHT); // 将燈帶控制引脚连接到通道3
    ledcSetup(PWM_CHANNEL_LIGHT, 200, 8); // 设置通道3的频率为200Hz，分辨率为8位

    ctrlCmdQueue = xQueueCreate(10, sizeof(ControlCommand*));
    Serial.println("[CTRL] Control command queue initialized.");
    return CANWAIFU_OK;
      // 初始化 PWM
  
}

void control_task(void *pvParameters) {
    ControlCommand* ctrlCmd;
    Serial.println("[CTRL] Control Loop Task started.");
    while (true) {
        if (xQueueReceive(ctrlCmdQueue, &ctrlCmd, portMAX_DELAY) == pdPASS) {
            Serial.print("[CTRL] ");
            ctrlCmd->print();
            switch (ctrlCmd->_mode)
            {
            case CTRL_MODE_ABS:
                enablePIDControl = false; // 禁用PID控制
                Serial.println("[CTRL] PID control disabled.");
                ledPWM = ctrlCmd->_led; // 更新LED PWM值
                fanPWM = ctrlCmd->_fan; // 更新风扇PWM值
                ledcWrite(PWM_CHANNEL_LIGHT, ledPWM); // 控制LED亮度
                ledcWrite(PWM_CHANNEL_FAN, fanPWM); // 控制风扇速度
                break;

            case CTRL_MODE_PID:
                targetLightIntensity = ctrlCmd->_lightIntensity; // 更新目标光照强度
                targetTemperature = ctrlCmd->_temperature; // 更新目标温度
                enablePIDControl = true; // 启用PID控制
                Serial.println("[CTRL] PID control enabled.");
                break;
            
            default:
                break;
            }
            delete ctrlCmd;  // 用完记得释放内存
        }
    }
}

void led_control_task(void *pvParameters) {
  Serial.println("[CTRL] LED Control Task started.");
  uint16_t currentLightIntensity = 0;
  int16_t error = 0;
  int16_t lastError = 0;     // 上一周期的誤差（新增）
  int16_t derivative = 0;    // 微分項（新增）
  int16_t integral = 0;      // 積分項

  while (true) {
    // Step 1: 获取当前光照值（线程安全）
    if (xSemaphoreTake(cropWaifuSensorsMutex, portMAX_DELAY) == pdTRUE) {
      currentLightIntensity = cropWaifuSensors.lightIntensity;
      cropWaifuSensors.ledPWM = ledPWM; // 更新LED PWM值到传感器对象
      xSemaphoreGive(cropWaifuSensorsMutex);
    }

    if (enablePIDControl) {
      error = (int16_t)(targetLightIntensity - currentLightIntensity);

      // 积分计算（带死区）
      if (abs(error) < PID_LED_DEADZONE) {
        integral += error;
        integral = constrain(integral, -5000, 5000); // 积分限幅
      }

      // 微分项（差分法）
      derivative = error - lastError;

      // PID 控制输出
      int controlOutput = (int)(
        (KP_LED * error + KI_LED * integral + KD_LED * derivative) / 1000
      );

      // 更新PWM
      int tempPWM = ledPWM + controlOutput;
      ledPWM = constrain(tempPWM, PWM_MIN, PWM_MAX);

      ledcWrite(PWM_CHANNEL_LIGHT, ledPWM);

      lastError = error;  // 保存当前误差为下一轮的 lastError
    }

    vTaskDelay(LED_CONTROL_INTERVAL_MS / portTICK_PERIOD_MS); // 每10ms调整一次LED亮度
  }
}


void fan_control_task(void *pvParameters) {
  float currentTemperature = 0.0f;
  int16_t error = 0;
  Serial.println("[CTRL] Fan Control Task started.");

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
      ledcWrite(PWM_CHANNEL_FAN, fanPWM); // 控制风扇速度
      // Serial.printf("[FAN] PWM: %d\n", fanPWM);
    }
    vTaskDelay(FAN_CONTROL_INTERVAL_MS / portTICK_PERIOD_MS); // 每10ms调整一次风扇速度
  }
}