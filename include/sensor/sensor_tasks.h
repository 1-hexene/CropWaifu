#ifndef SENSOR_TASKS_H
#define SENSOR_TASKS_H

#include <sensor/cropwaifu_sensors.h>

canwaifu_status sensor_init(void);
void sensor_task(void *pvParameters);

#endif // SENSOR_TASKS_H