#include <can_fd_tasks.h>
#include <twai_tasks.h>
#include <wifi_tasks.h>
#include <webserver_tasks.h>
#include <timer_tasks.h>

void setup()
{

  vTaskDelay(1000 / portTICK_PERIOD_MS);
  Serial.begin(115200);
  Serial.print("CanWaifu Software Version: ");
  Serial.println(SW_VER);
  // 如果初始化炸掉了 （其中有一个的结果是1）
  if (can_fd_init() | twai_init() | wifi_init() | webserver_init()| timer_init())
  {
    // 就把红灯打开
    digitalWrite(PIN_LED_ERR, HIGH);
  }
  else
  {
    //全都正常的时候打开蓝灯
    #ifdef HW_VER_1C
      digitalWrite(PIN_LED_B, HIGH);
    #endif
    
    //开始注册服务
    // CANFD 相关任务
    xTaskCreate(can_fd_receive_task, "can_fd_receive_task", 8192, NULL, 14, NULL); // 优先级提升
    Serial.println("[Main] Created CANFD receive service");
    // xTaskCreate(can_fd_send_task, "can_fd_send_task", 4096, NULL, 11, NULL);
    // Serial.println("[Main] Created CANFD send service");
    xTaskCreate(can_fd_reset_frequency_task, "Reset Frequency", 2048, NULL, 15, NULL); // 新增
    Serial.println("[Main] Created CANFD frequency reset service");

    // TWAI 相关任务
    xTaskCreate(twai_send_task, "twai send task", 4096, NULL, 12, NULL);
    Serial.println("[Main] Created TWAI send service");
    xTaskCreate(twai_receive_task, "twai receive task", 4096, NULL, 13, NULL);
    Serial.println("[Main] Created TWAI receive service");
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