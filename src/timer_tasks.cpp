#include <timer_tasks.h>

hw_timer_t *timer = NULL;
SemaphoreHandle_t mqttHeartBeatSignal = xSemaphoreCreateBinary();

canwaifu_status timer_init(){
    // 定时器初始化
    timer = timerBegin(0, 8000, true); // Timer 0, prescaler 80 (1us per tick)
    timerAttachInterrupt(timer, &mqtt_heartbeat_ISR, true);
    timerAlarmWrite(timer, 10000, true); // 1000 ms interval
    timerAlarmEnable(timer);              // Enable the alarm
    return CANWAIFU_OK;
}


// This ISR is called every 1000 ms to signal the MQTT heartbeat
void IRAM_ATTR mqtt_heartbeat_ISR()
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xSemaphoreGiveFromISR(mqttHeartBeatSignal, &xHigherPriorityTaskWoken);
    if (xHigherPriorityTaskWoken == pdTRUE)
    {
        portYIELD_FROM_ISR();
    }
}