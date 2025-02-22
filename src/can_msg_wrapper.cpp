#include <can_msg_wrapper.h>

CANFDMessage CanMsgWrapper::getCanFdMsgContent(){
    return canFdMsg;
}

uint8_t CanMsgWrapper::resetCount(){
    frequency = 0;
    return frequency;
}

uint8_t CanMsgWrapper::getFrequency(){
    return frequency;
}

uint8_t CanMsgWrapper::countPlusOne(){
    frequency ++;
    return frequency;
}