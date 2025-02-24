#ifndef CAN_MSG_WRAPPER_H
#define CAN_MSG_WRAPPER_H
#include <Arduino.h>
#include <ACAN2517FD.h>

class CanMsgWrapper
{
private:
    CANFDMessage canFdMsg;
    uint8_t frequency;         // 当前频率
    uint8_t lastFrequency;     // 上一次清零前的频率
public:
    CANFDMessage getCanFdMsgContent();
    uint8_t resetCount();
    uint8_t getFrequency();    // 返回上一次清零前的频率
    uint8_t countPlusOne();
    void updateMessage(const CANFDMessage &msg); // 新增方法
};

#endif