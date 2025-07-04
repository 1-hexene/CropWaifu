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
  }
  else
  {
  }
}

void loop()
{
  vTaskDelay(1000 / portTICK_PERIOD_MS);
}