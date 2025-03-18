#include <can_msg_wrapper.h>

static CanMsgWrapper canMsgWrapperList[63];
SemaphoreHandle_t canMsgMutex = xSemaphoreCreateMutex();

CANFDMessage CanMsgWrapper::getCanFdMsgContent() {
    return canFdMsg;
}

uint16_t CanMsgWrapper::resetCount() {
    lastFrequency = frequency; // 清零前保存当前频率
    frequency = 0;
    return frequency;
}

uint16_t CanMsgWrapper::getCurrentFrequency() {
    return frequency * 10; // 返回上一次清零前的频率
}

uint16_t CanMsgWrapper::getFrequency() {
    return lastFrequency * 10; // 返回上一次清零前的频率
}

uint16_t CanMsgWrapper::countPlusOne() {
    frequency++;
    return frequency;
}

void CanMsgWrapper::updateMessage(const CANFDMessage &msg) {
    canFdMsg = msg; // 更新报文内容
}

CanMsgWrapper *getCanMsgWrapperList()
{
    return canMsgWrapperList;
}

uint8_t getCanMsgWrapperListLen(){
    return sizeof(canMsgWrapperList)/sizeof(canMsgWrapperList[0]);
}
