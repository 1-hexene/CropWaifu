#ifndef TWAI_TASKS_H
#define TWAI_TASKS_H

#include <Arduino.h>
#include <SPI.h>
#include <ACAN2517FD.h>
#include <pins.h>
#include <ESP32-TWAI-CAN.hpp>

void print_twai_message(HardwareSerial *_hardwareSerial, CanFrame *canFrame, bool direction_is_send);
canwaifu_status twai_init();
void twai_send_task(void *pvParameters);
void twai_receive_task(void *pvParameters);


#endif