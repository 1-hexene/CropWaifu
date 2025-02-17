#ifndef CAN_FD_TASKS_H
#define CAN_FD_TASKS_H

#include <SPI.h>
#include <ACAN2517FD.h>
#include "pins.h"

void print_can_fd_message(HardwareSerial *_hardwareSerial, CANFDMessage canFdMessage);
void can_fd_init();
void can_fd_send_task(void *);
void can_fd_receive_task(void *);

#endif