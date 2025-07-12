#ifndef SENSOR_TASKS_H
#define SENSOR_TASKS_H

#include <canwaifu_base.h>
#include <sensor/bh1750fvi_lib.h>
#include <sensor/cropwaifu_sensors.h>
#include <SensirionI2cSht3x.h>

canwaifu_status sensor_init(void);
void sensor_task(void *pvParameters);

#endif // SENSOR_TASKS_H