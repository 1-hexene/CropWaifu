#ifndef CANWAIFU_BASE_H
#define CANWAIFU_BASE_H

#include <Arduino.h>

/* BSP Pins */

// LED Pins
#define PIN_LED_R 3
#define PIN_LED_G 5
#define PIN_LED_B 8

// I2C Pins
#define PIN_I2C_SDA 0
#define PIN_I2C_SCL 1

// Peripherals Reset Pins
#define PIN_SHT30_RST 4
#define PIN_BH1750_RST 2

// Fan Pins
#define PIN_FAN_CON 6
#define PIN_FAN_INT 7

// Light Stripe Pins
#define PIN_LIGHT_CON 10

enum canwaifu_status{
    CANWAIFU_OK = 0,
    CANWAIFU_ERR = 1
};

#endif