#include <main.h>
#include <driver/gpio.h>

void setup()
{
  rgb_led_init(); // 初始化RGB LED
  vTaskDelay(3000 / portTICK_PERIOD_MS);
  Serial.begin(115200);
  Serial.print("CropWaifu Software Version: ");
  Serial.println(SW_VER);

  if (wifi_init() | // Initialize WiFi
      control_init()|
      timer_init()|  // Initialize timer
      cropwaifu_ble_init() | 
      sensor_init()
  )
  {
    rgb_led_set_color(255, 0, 0); // red on error
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    Serial.println("[MAIN] Initialization failed, restarting...");
    ESP.restart();
  }
  
  mqtt_init();

  vTaskDelay(1000 / portTICK_PERIOD_MS);

  Serial.println("[MAIN] Registering tasks...");
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  xTaskCreate (mqtt_loop_task, "MqttLoopTask", 4096, NULL, 1, NULL);
  xTaskCreate (control_task, "ControlTask", 4096, NULL, 1, NULL);
  xTaskCreate (ble_task,"BLE Task", 4096,NULL, 1, NULL);
  xTaskCreate (cropwaifu_daemon,"Cropwaifu Daemon", 4096, NULL, 1, NULL);
  xTaskCreate (sensor_task, "SensorTask", 4096, NULL, 1, NULL);
  Serial.println("[MAIN] Control task created");

}

void loop()
{
  vTaskDelay(1000 / portTICK_PERIOD_MS); // Main loop does nothing, just delay
}