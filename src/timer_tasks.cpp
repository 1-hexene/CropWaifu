#include <timer_tasks.h>

hw_timer_t *timer = NULL;
SemaphoreHandle_t canFreqReset = xSemaphoreCreateBinary();

canwaifu_status timer_init(){
    // 定时器初始化
    timer = timerBegin(0, 8000, true); // Timer 0, prescaler 80 (1us per tick)
    timerAttachInterrupt(timer, &can_fd_reset_freq_ISR, true);
    timerAlarmWrite(timer, 10000, true); // 100 ms interval
    timerAlarmEnable(timer);              // Enable the alarm
    return CANWAIFU_OK;
}

void IRAM_ATTR can_fd_reset_freq_ISR()
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xSemaphoreGiveFromISR(canFreqReset, &xHigherPriorityTaskWoken);
    if (xHigherPriorityTaskWoken == pdTRUE)
    {
        portYIELD_FROM_ISR();
    }
}