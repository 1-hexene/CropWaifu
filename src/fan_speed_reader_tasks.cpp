#include <sensor/fan_speed_reader_tasks.h>

volatile uint16_t fanSpeed = 0; // maximum speed is about 3400
portMUX_TYPE fan_speed_mux = portMUX_INITIALIZER_UNLOCKED;
extern CropWaifuSensors cropWaifuSensors; // From sensor_tasks.cpp

canwaifu_status fan_speed_reader_init() {
    //TODO: try INPUT_PULLUP to see if the pull-up resistor is needed
    pinMode(PIN_FAN_INT, INPUT);
    attachInterrupt(PIN_FAN_INT, fan_speed_reader_ISR, RISING);
    return CANWAIFU_OK;
}

void IRAM_ATTR fan_speed_reader_ISR() {
    portENTER_CRITICAL_ISR(&fan_speed_mux);
    fanSpeed++;
    portEXIT_CRITICAL_ISR(&fan_speed_mux);
}

void fan_speed_reader_task(void *parameter) {
  uint16_t currCount = 0;
  while (true) {
    // 为防止并发冲突，复制一个副本
    portENTER_CRITICAL(&fan_speed_mux);
    uint16_t currCount = fanSpeed;
    fanSpeed = 0; // 重置计数器
    portEXIT_CRITICAL(&fan_speed_mux);

    Serial.printf("[FANS] Interrupt count: %d\n", currCount);
    cropWaifuSensors.fanSpeedRPM = currCount * 59; // Convert to RPM

    vTaskDelay(pdMS_TO_TICKS(1017)); // 1017ms延时
  }
}