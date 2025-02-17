#include <Arduino.h>
#include <iostream>
#include "pins.h"
#include "can_fd_tasks.h"

void setup()
{
  can_fd_init();
  xTaskCreate(can_fd_receive_task, "can_fd_receive_task", 4096, NULL, 10, NULL);
  Serial.println("[Main] Created receive service");
  xTaskCreate(can_fd_send_task, "can_fd_send_task", 4096, NULL, 11, NULL);
  Serial.println("[Main] Created send service");
}



void loop()
{
  vTaskDelay(1000/portTICK_PERIOD_MS);
}
// END FILE