#include <main.h>

extern canwaifu_status globalStatus; // From cropwaifu_daemon.cpp

void setup()
{
  rgb_led_init(); // 初始化RGB LED
  Serial.begin(115200);
  uint8_t i = 1;
  Serial.println("CropWaifu ready to start. \nWaiting 3s for peripherals to power up....");
  while ( i <= 3 ) {
    Serial.printf("%d...", i);
    i ++;
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
  
  Serial.print("\nCropWaifu Software Version: ");
  Serial.println(SW_VER);

  if (control_init()|
      timer_init()|  // Initialize timer
      cropwaifu_ble_init() | 
      sensor_init() |
      fan_speed_reader_init()| // Initialize fan speed reader
      wifi_init() // Initialize WiFi
  )
  {
    rgb_led_set_color(255, 0, 0); // red on error
    Serial.println("[MAIN] Initialization failed, restarting...");
    vTaskDelay(1000 / portTICK_PERIOD_MS);

    ESP.restart();
  } else {
    Serial.println("[MAIN] Peripherial initialization successful.");
  }

  Serial.println("[MAIN] Starting MQTT...");
  
  mqtt_init();

  vTaskDelay(1000 / portTICK_PERIOD_MS);

  Serial.println("[MAIN] Registering tasks...");
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  xTaskCreate (mqtt_loop_task, "MqttLoopTask", 4096, NULL, 1, NULL);
  xTaskCreate (control_task, "ControlTask", 4096, NULL, 1, NULL);
  xTaskCreate (ble_task,"BLE Task", 4096,NULL, 1, NULL);
  xTaskCreate (cropwaifu_daemon,"Cropwaifu Daemon", 4096, NULL, 1, NULL);
  xTaskCreate (sensor_task, "SensorTask", 4096, NULL, 1, NULL);
  xTaskCreate (led_control_task, "LedControlTask", 4096, NULL, 1, NULL);
  xTaskCreate (fan_speed_reader_task, "FanSpeedReaderTask", 4096, NULL, 1, NULL);
  xTaskCreate (fan_control_task, "FanControlTask", 4096, NULL, 1, NULL);
  Serial.println("[MAIN] Control task created");

}

void loop()
{
  vTaskDelay(1000 / portTICK_PERIOD_MS); // Main loop does nothing, just delay
}