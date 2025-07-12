#include <control/cropwaifu_daemon.h>

canwaifu_status globalStatus = CANWAIFU_OK; // 全局状态变量


void rgb_led_init() {
    pinMode(PIN_LED_R, OUTPUT);
    pinMode(PIN_LED_G, OUTPUT);
    pinMode(PIN_LED_B, OUTPUT);
    analogWrite(PIN_LED_R, 0);
    analogWrite(PIN_LED_G, 0);
    analogWrite(PIN_LED_B, 255);
}

void rgb_led_set_color(uint8_t r, uint8_t g, uint8_t b) {
    analogWrite(PIN_LED_R, r);
    analogWrite(PIN_LED_G, g);
    analogWrite(PIN_LED_B, b);
}

void rgb_led_blink(uint8_t r, uint8_t g, uint8_t b, uint32_t duration) {
    rgb_led_set_color(r, g, b);
    vTaskDelay(duration / portTICK_PERIOD_MS);
    rgb_led_off();
}

void rgb_led_off() {
    analogWrite(PIN_LED_R, 0);
    analogWrite(PIN_LED_G, 0);
    analogWrite(PIN_LED_B, 0);
}

void cropwaifu_daemon(void *pvParameters) {
    Serial.println("[DMON] RGB LED Task started");
    uint8_t errorCount = 0;

    while (true) {
        // 示例：每隔1秒变换颜色
        switch (globalStatus)
        {
        case CANWAIFU_OK:
            rgb_led_set_color(0, 255, 0); // 绿色
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
            Serial.println("[DMON] Too many errors, restarting...");
            ESP.restart();
        }
        vTaskDelay(100 / portTICK_PERIOD_MS); // update per 100ms
    }
}