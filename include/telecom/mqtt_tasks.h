#ifndef MQTT_TASKS_H
#define MQTT_TASKS_H

#include <canwaifu_base.h>
#include <EspMQTTClient.h>
#include <ArduinoJson.h>
#include <control/control_tasks.h>

void mqtt_init(void);
void mqtt_loop_task(void *parameter);
void mqtt_message_receive_callback(const String &payload);
void mqtt_heartbeat_task(void *pvParameters);

#endif