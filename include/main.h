#ifndef MAIN_H
#define MAIN_H

#include <canwaifu_base.h>
#include <timer_tasks.h>

#include <sensor/sensor_tasks.h>

#include <control/control_tasks.h>
#include <control/cropwaifu_daemon.h>

#include <telecom/mqtt_tasks.h>
#include <telecom/wifi_tasks.h>
#include <telecom/ble_tasks.h>

void setup();
void loop();

#endif // MAIN_H