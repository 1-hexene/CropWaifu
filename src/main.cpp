#include <wifi_tasks.h>
#include <timer_tasks.h>

void setup()
{

  vTaskDelay(1000 / portTICK_PERIOD_MS);
  Serial.begin(115200);
  Serial.print("CanWaifu Software Version: ");
  Serial.println(SW_VER);
  // 如果初始化炸掉了 （其中有一个的结果是1）
  if (wifi_init() | timer_init())
  {
    // 就把红灯打开
    digitalWrite(PIN_LED_ERR, HIGH);
  }
  else
  {
  }
}

void loop()
{
  vTaskDelay(1000 / portTICK_PERIOD_MS);
}

void led_setup()
{
#if defined(HW_VER_1A) || defined(HW_VER_1AR2) || defined(HW_VER_1B)
  pinMode(PIN_CANFD_STATUS, OUTPUT);
  pinMode(PIN_TWAI_STATUS, OUTPUT);
  pinMode(PIN_WIFI_STATUS, OUTPUT);
#endif

#ifdef HW_VER_1C
  pinMode(PIN_LED_R, OUTPUT);
  pinMode(PIN_LED_G, OUTPUT);
  pinMode(PIN_LED_B, OUTPUT);
#endif
}
// END FILE