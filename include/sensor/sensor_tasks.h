#ifndef SENSOR_TASKS_H
#define SENSOR_TASKS_H

#include <sensor/cropwaifu_sensors.h>
#include <sensor/bh1750fvi_lib.h>
#include <SHT85.h>

canwaifu_status sensor_init(void);
void sensor_task(void *pvParameters);

#endif // SENSOR_TASKS_H