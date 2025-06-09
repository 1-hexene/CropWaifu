#ifndef CAN_FD_TASKS_H
#define CAN_FD_TASKS_H

#include <canwaifu_base.h>
#include <SPI.h>
#include <ACAN2517FD.h>
#include <can_msg_wrapper.h>
#include <timer_tasks.h>

void can_fd_reset_frequency_task(void *pvParameters);
void print_can_fd_message(HardwareSerial* _hardwareSerial, CANFDMessage* canFdMessage);
canwaifu_status can_fd_init();
canwaifu_status can_fd_init(uint8_t oscFreq, uint32_t arbitrationBitRate, uint8_t dataBitRatefactor, uint8_t mode);
void can_fd_send_task(void *);
void can_fd_receive_task(void *);
void IRAM_ATTR can_fd_ISR();

#endif