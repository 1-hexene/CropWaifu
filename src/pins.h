/* 定义引脚头文件 */
/* Autho : Bababoi */
/* Date  : 18 Feb 2025 */
#ifndef PINS_H
#define PINS_H

/* CANFD Pins */
#ifdef HW_VER_1A //没飞过线的
#define SOFT_SCK_PIN   6
#define SOFT_MISO_PIN  7
#define SOFT_MOSI_PIN  2
#endif

#ifdef HW_VER_1B //飞过线的那块，理论SPI延迟更低，但是飞过线了信号可能更不稳定
#define SOFT_SCK_PIN   6
#define SOFT_MISO_PIN  2
#define SOFT_MOSI_PIN  7
#endif

#define MCP2518FD_CHIP_SELECT 10 
#define PIN_CANFD_STDBY    8

/* TWAI Pins */
#define PIN_TWAI_RX        0
#define PIN_TWAI_TX        1
#define PIN_TWAI_STDBY     3

#endif