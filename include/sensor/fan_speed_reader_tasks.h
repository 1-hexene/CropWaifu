#ifndef FAN_SPEED_TASKS_H
#define FAN_SPEED_TASKS_H

#include <canwaifu_base.h>
#include <sensor/cropwaifu_sensors.h>

canwaifu_status fan_speed_reader_init();
void IRAM_ATTR fan_speed_reader_ISR();
void fan_speed_reader_task(void *parameter);

#endif // FAN_SPEED_TASKS_H