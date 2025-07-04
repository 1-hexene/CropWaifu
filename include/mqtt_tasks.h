#ifndef MQTT_TASKS_H
#define MQTT_TASKS_H

#include <canwaifu_base.h>
#include <EspMQTTClient.h>
#include <ArduinoJson.h>
#include <control_tasks.h>

canwaifu_status mqtt_init(void);
void mqtt_loop_task(void *parameter);

#endif