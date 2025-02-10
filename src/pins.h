#include <Arduino.h>

#ifdef HW_VER_1A
const uint8_t SOFT_SCK_PIN  = 6;
const uint8_t SOFT_MISO_PIN = 7;
const uint8_t SOFT_MOSI_PIN = 2;
#define SELECT 10 
#endif

#ifdef HW_VER_1B
const uint8_t SOFT_SCK_PIN  = 6;
const uint8_t SOFT_MISO_PIN = 2;
const uint8_t SOFT_MOSI_PIN = 7;
#define SELECT 10 
#endif