#include <main.h>

void setup()
{
  vTaskDelay(3000 / portTICK_PERIOD_MS);
  Serial.begin(115200);
  Serial.print("CropWaifu Software Version: ");
  Serial.println(SW_VER);
  wifi_init();
  control_init();
  mqtt_init();
 
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  Serial.println("[Main] Registering tasks...");
  xTaskCreate (mqtt_loop_task, "MqttLoopTask", 4096, NULL, 1, NULL);
  xTaskCreate (control_task, "ControlTask", 4096, NULL, 1, NULL);
  Serial.println("[Main] Control task created");
}

void loop()
{
  vTaskDelay(1000 / portTICK_PERIOD_MS);
}