#include <timer_tasks.h>

hw_timer_t *mqttHeartbeatTimer = NULL;
hw_timer_t *bleUpdateTimer = NULL;
SemaphoreHandle_t mqttHeartBeatSignal = xSemaphoreCreateBinary();
SemaphoreHandle_t bleUpdateSignal = xSemaphoreCreateBinary();

// ESP32C3 Has 2 timers
canwaifu_status timer_init(){
    // 定时器0初始化
    mqttHeartbeatTimer = timerBegin(0, 8000, true); // Timer 0, prescaler 8000 (0.1ms per tick)
    timerAttachInterrupt(mqttHeartbeatTimer, &mqtt_heartbeat_ISR, true);
    timerAlarmWrite(mqttHeartbeatTimer, 10000, true); // 1000 ms interval
    timerAlarmEnable(mqttHeartbeatTimer);              // Enable the alarm

    //  定时器1初始化
    bleUpdateTimer = timerBegin(1, 26667, true); // Timer 1, prescaler 26667 (0.33ms per tick)
    timerAttachInterrupt(bleUpdateTimer, &ble_update_ISR, true);
    timerAlarmWrite(bleUpdateTimer, 1000, true); // 1000 ms interval
    timerAlarmEnable(bleUpdateTimer);              // Enable the alarm
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

// This ISR is called every 1000 ms to signal the ble update characteristic
void IRAM_ATTR ble_update_ISR()
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xSemaphoreGiveFromISR(bleUpdateSignal, &xHigherPriorityTaskWoken);
    if (xHigherPriorityTaskWoken == pdTRUE)
    {
        portYIELD_FROM_ISR();
    }
}