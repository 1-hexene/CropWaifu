#ifndef RGB_LED_H
#define RGB_LED_H

#include <canwaifu_base.h>

void rgb_led_init(); // Initialize RGB LED
void rgb_led_set_color(uint8_t r, uint8_t g, uint8_t b); // Set RGB LED color
void rgb_led_blink(uint8_t r, uint8_t g, uint8_t b, uint32_t duration); // Blink RGB LED with specified color and duration
void rgb_led_off(); // Turn off RGB LED

void rgb_led_task(void *pvParameters);

#endif // RGB_LED_H