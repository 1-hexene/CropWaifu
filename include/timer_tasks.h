#ifndef TIMER_TASKS_H
#define TIMER_TASKS_H

#include <canwaifu_base.h>

canwaifu_status timer_init();
void IRAM_ATTR mqtt_heartbeat_ISR();
#endif