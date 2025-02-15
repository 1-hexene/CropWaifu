#include <Arduino.h>

#ifdef HW_VER_1A
#define SOFT_SCK_PIN   6
#define SOFT_MISO_PIN  7
#define SOFT_MOSI_PIN  2
#define SELECT 10 
#endif

#ifdef HW_VER_1B
#define SOFT_SCK_PIN   6
#define SOFT_MISO_PIN  2
#define SOFT_MOSI_PIN  7
#define SELECT 10
#endif