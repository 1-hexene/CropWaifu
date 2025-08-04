#ifndef CANWAIFU_BASE_H
#define CANWAIFU_BASE_H

#include <Arduino.h>

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

#define SERVICE_UUID        "11451411-4514-1145-1411-00000000000" TOSTRING(BOARD_ID)
#define CHARACTERISTIC_UUID "19198101-9198-1019-1981-00000000000" TOSTRING(BOARD_ID)
#define MQTT_CLIENT_NAME    "CropWaifu-Board-" TOSTRING(BOARD_ID)
#define BLE_ADVERTISE_NAME  "CropWaifu-" TOSTRING(BOARD_ID)


/* BSP Pins */

// LED Pins
#define PIN_LED_R 5
#define PIN_LED_G 8
#define PIN_LED_B 3

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

/*PWM Channels*/
#define PWM_CHANNEL_LED_R 0
#define PWM_CHANNEL_LED_G 1
#define PWM_CHANNEL_LED_B 2
#define PWM_CHANNEL_LIGHT 3
#define PWM_CHANNEL_FAN 4

enum canwaifu_status{
    CANWAIFU_OK = 0,
    CANWAIFU_WARNING = 1,
    CANWAIFU_ERR = 3 // 0b11
};

#endif