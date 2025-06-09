#include <can_msg_wrapper.h>

static CanMsgWrapper canMsgWrapperList[64];
static CanMsgWrapper twaiMsgWrapperList[64];

SemaphoreHandle_t canMsgMutex = xSemaphoreCreateMutex();
SemaphoreHandle_t twaiMSgMutex = xSemaphoreCreateMutex();

uint8_t* CanMsgWrapper::getContent() {
    return msgContent;
}

uint32_t CanMsgWrapper::getID() {
    return msgID;
}

uint8_t CanMsgWrapper::getLength() {
    return length;
}

uint8_t CanMsgWrapper::getType() {
    return type;
}

uint16_t CanMsgWrapper::resetCount() {
    lastFrequency = frequency; // 清零前保存当前频率
    frequency = 0;
    return frequency;
}

void CanMsgWrapper::triggerCacheContent(){
    cacheNextContent = true;
}

uint16_t CanMsgWrapper::getCurrentFrequency() {
    return frequency; // 返回上一次清零前的频率
}

uint16_t CanMsgWrapper::getLastFrequency() {
    return lastFrequency; // 返回上一次清零前的频率
}

uint16_t CanMsgWrapper::countPlusOne() {
    frequency++;
    return frequency;
}

void CanMsgWrapper::updateMessage(const CANFDMessage &msg) {
    if (cacheNextContent) {
        memcpy(msgContent, msg.data, sizeof(msg.data));
        cacheNextContent = false;
    }
    msgID = msg.id;
    length = msg.len;
    type = msg.type;
}

void CanMsgWrapper::updateMessage(const CanFrame &twaiMsg) {
    if (cacheNextContent) {
        memcpy(msgContent, twaiMsg.data, sizeof(twaiMsg.data));
        cacheNextContent = false;
    }
    msgID = twaiMsg.identifier;
    length = twaiMsg.data_length_code;
    type = (twaiMsg.rtr)? Type::CAN_REMOTE : Type::CAN_DATA;

}

CanMsgWrapper *getCanMsgWrapperList()
{
    return canMsgWrapperList;
}

CanMsgWrapper *getTwaiMsgWrapperList()
{
    return twaiMsgWrapperList;
}

uint8_t getCanMsgWrapperListLen(){
    return sizeof(canMsgWrapperList)/sizeof(canMsgWrapperList[0])  - 1;
}
