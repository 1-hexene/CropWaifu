/* 定义引脚头文件 */
/* Autho : Bababoi */
/* Date  : 18 Feb 2025 */
#ifndef PINS_H
#define PINS_H

enum canwaifu_status{
    CANWAIFU_OK = 0,
    CANWAIFU_ERR = 1
};

/* MCP2518 PINS */
#define MCP2518FD_CHIP_SELECT 10

/* TWAI Pins */
#define PIN_TWAI_RX 0
#define PIN_TWAI_TX 1

/* BSP Pins */
#ifdef HW_VER_1A // 没飞过线的
#define SOFT_SCK_PIN 6
#define SOFT_MISO_PIN 7
#define SOFT_MOSI_PIN 2
#define PIN_CANFD_STATUS 8
#define PIN_TWAI_STATUS 3
#define PIN_WIFI_STATUS 8
#define PIN_LED_ERR 8
#define PIN_MCP2518_INT 4
#endif

#ifdef HW_VER_1AR2 // 没飞过线的
#define SOFT_SCK_PIN 6
#define SOFT_MISO_PIN 7
#define SOFT_MOSI_PIN 2
#define PIN_CANFD_STATUS 8
#define PIN_TWAI_STATUS 3
#define PIN_WIFI_STATUS 8
#define PIN_LED_ERR 8
#define PIN_MCP2518_INT 5
#endif

#ifdef HW_VER_1B // 飞过线的那块，理论SPI延迟更低，但是飞过线了信号可能更不稳定
#define SOFT_SCK_PIN 6
#define SOFT_MISO_PIN 2
#define SOFT_MOSI_PIN 7
#define PIN_CANFD_STATUS 8
#define PIN_TWAI_STATUS 3
#define PIN_WIFI_STATUS 8
#define PIN_LED_ERR 8
#define PIN_MCP2518_INT 4
#endif

#ifdef HW_VER_1C // 28/02/2025 船新版本
#define SOFT_SCK_PIN 6
#define SOFT_MISO_PIN 2
#define SOFT_MOSI_PIN 7

#define PIN_LED_R 3
#define PIN_LED_G 5
#define PIN_LED_B 8

#define PIN_CANFD_STATUS PIN_LED_R
#define PIN_TWAI_STATUS PIN_LED_R
#define PIN_WIFI_STATUS PIN_LED_R
#define PIN_LED_ERR PIN_LED_R
#define PIN_MCP2518_INT 4
#endif

#endif