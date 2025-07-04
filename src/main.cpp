#include <timer_tasks.h>
#include <Wire.h>
#include <EspMQTTClient.h>

EspMQTTClient client(
  "F429", // WiFi SSID
  "FF442299", // WiFi Password
  "10.0.0.171", // MQTT server IP
  "CropWaifu-Test", // MQTT client name
  5001 // MQTT server port
);

void onConnectionEstablished() {

  client.subscribe("cropwaifu/test", [] (const String &payload)  {
    Serial.println(payload);
  });

  client.publish("cropwaifu/test", "This is a message");
}

void setup()
{
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  Serial.begin(115200);
  Serial.print("CropWaifu Software Version: ");
  Serial.println(SW_VER);
  // 如果初始化炸掉了 （其中有一个的结果是1）
  if (timer_init()){}
}

void loop()
{
  client.loop(); // 处理MQTT连接
  vTaskDelay(1000 / portTICK_PERIOD_MS);
}