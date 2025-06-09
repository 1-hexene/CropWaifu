#ifndef CAN_MSG_WRAPPER_H
#define CAN_MSG_WRAPPER_H
#include <Arduino.h>
#include <ACAN2517FD.h>
#include <ESP32-TWAI-CAN.hpp>
#define TWAI_MSG_LENGTH 8
#define CANFD_MSG_LENGTH 64

class CanMsgWrapper
{
    private:
        //CANFDMessage canFdMsg;
        uint32_t msgID; // For Canfd
        uint8_t msgContent[CANFD_MSG_LENGTH];
        uint8_t  length;
        uint8_t  type;
        uint16_t frequency;         // 当前频率
        uint16_t lastFrequency;     // 上一次清零前的频率
        bool cacheNextContent = true;

    public:
        typedef enum : uint8_t {
            CAN_REMOTE,
            CAN_DATA,
            CANFD_NO_BIT_RATE_SWITCH,
            CANFD_WITH_BIT_RATE_SWITCH
        } Type ;
        
        uint8_t* getContent();
        uint32_t getID();

        uint8_t  getLength();
        uint8_t  getType();

        uint16_t resetCount();
        uint16_t getLastFrequency();    // 返回上一次清零前的频率
        uint16_t getCurrentFrequency();
        uint16_t countPlusOne();

        void triggerCacheContent();

        void updateMessage(const CANFDMessage &msg); // 新增方法
        void updateMessage(const CanFrame &twaiMsg);
};

CanMsgWrapper *getCanMsgWrapperList();
CanMsgWrapper *getTwaiMsgWrapperList();
uint8_t getCanMsgWrapperListLen();

#endif