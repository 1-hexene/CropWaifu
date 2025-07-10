#include <control/rgb_led.h>

canwaifu_status globalStatus = CANWAIFU_OK; // 全局状态变量

void rgb_led_init() {
    pinMode(PIN_LED_R, OUTPUT);
    pinMode(PIN_LED_G, OUTPUT);
    pinMode(PIN_LED_B, OUTPUT);
    rgb_led_off(); // 初始化时关闭LED
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

void rgb_led_task(void *pvParameters) {
    Serial.println("[RGB LED] RGB LED Task started");
    rgb_led_init(); // 初始化RGB LED

    while (true) {
        // 示例：每隔1秒变换颜色
        rgb_led_set_color(255, 0, 0); // 红色
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        rgb_led_set_color(0, 255, 0); // 绿色
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        rgb_led_set_color(0, 0, 255); // 蓝色
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}