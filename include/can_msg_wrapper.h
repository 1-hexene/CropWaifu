#ifndef CAN_MSG_WRAPPER_H
#define CAN_MSG_WRAPPER_H
#include <Arduino.h>
#include <ACAN2517FD.h>

class CanMsgWrapper
{
private:
    CANFDMessage canFdMsg;
    uint8_t frequency;
public:
    CANFDMessage getCanFdMsgContent();
    uint8_t resetCount();
    uint8_t getFrequency();
    uint8_t countPlusOne();
};

#endif