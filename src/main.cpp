#include "can_fd_tasks.h"
#include "twai_tasks.h"
#include "wifi_tasks.h"

void setup()
{
  can_fd_init();
  twai_init();
  wifi_init();
  xTaskCreate(can_fd_receive_task, "can_fd_receive_task", 4096, NULL, 10, NULL);
  Serial.println("[Main] Created CANFD receive service");
  xTaskCreate(can_fd_send_task, "can_fd_send_task", 4096, NULL, 11, NULL);
  Serial.println("[Main] Created CANFD send service");
  xTaskCreate(twai_send_task, "twai send task", 4096, NULL, 12, NULL);
  Serial.println("[Main] Created TWAI send service");
  xTaskCreate(twai_receive_task, "twai receive task", 4096, NULL, 13, NULL);
  Serial.println("[Main] Created TWAI receive service");
}



void loop()
{
  vTaskDelay(1000/portTICK_PERIOD_MS);
}
// END FILE