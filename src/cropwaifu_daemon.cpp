#include <control/cropwaifu_daemon.h>

canwaifu_status globalStatus = CANWAIFU_OK; // 全局状态变量


void rgb_led_init() {
    pinMode(PIN_LED_R, OUTPUT);
    ledcAttachPin(PIN_LED_R, PWM_CHANNEL_LED_R); // 将LED紅燈控制引脚连接到通道0
    ledcSetup(PWM_CHANNEL_LED_R, 2000, 8); // 设置通道0的频率为2000Hz，分辨率为8位

    pinMode(PIN_LED_G, OUTPUT);
    ledcAttachPin(PIN_LED_G, PWM_CHANNEL_LED_G);
    ledcSetup(PWM_CHANNEL_LED_G, 2000, 8);

    pinMode(PIN_LED_B, OUTPUT);
    ledcAttachPin(PIN_LED_B, PWM_CHANNEL_LED_B);
    ledcSetup(PWM_CHANNEL_LED_B, 2000, 8);
    
    ledcWrite(PWM_CHANNEL_LED_R, 0);
    ledcWrite(PWM_CHANNEL_LED_G, 0);
    ledcWrite(PWM_CHANNEL_LED_B, 255);
}

void rgb_led_set_color(uint8_t r, uint8_t g, uint8_t b) {
    ledcWrite(PWM_CHANNEL_LED_R, r);
    ledcWrite(PWM_CHANNEL_LED_G, g);
    ledcWrite(PWM_CHANNEL_LED_B, b);

}

void rgb_led_blink(uint8_t r, uint8_t g, uint8_t b, uint32_t duration) {
    rgb_led_set_color(r, g, b);
    vTaskDelay(duration / portTICK_PERIOD_MS);
    rgb_led_off();
}

void rgb_led_off() {
    rgb_led_set_color(0, 0, 0); // 关闭RGB LED
}

void cropwaifu_daemon(void *pvParameters) {
    Serial.println("[DMON] RGB LED Task started");
    uint8_t errorCount = 0;

    while (true) {
        // 示例：每隔1秒变换颜色
        switch (globalStatus)
        {
        case CANWAIFU_OK:
            rgb_led_set_color(0, 128, 255); // 绿色
            break;
        case CANWAIFU_ERR:
            rgb_led_set_color(255, 0, 0); // 红色
            errorCount ++;
            break;
        case CANWAIFU_WARNING:
            rgb_led_set_color(255, 255, 0); // 黄色
        default:
            break;
        }

        if (errorCount >= 10) {
            Serial.println("[DMON] #######[CRITICAL ERROR]#######");
            Serial.println("[DMON] Too many errors, restarting...");
            ESP.restart();
        }
        vTaskDelay(100 / portTICK_PERIOD_MS); // update per 100ms
    }
}