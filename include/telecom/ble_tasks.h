#ifndef BLE_TAKS_H
#define BLE_TAKS_H

#include <canwaifu_base.h>
#include <sensor/cropwaifu_sensors.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>


canwaifu_status cropwaifu_ble_init();  // 创建 BLE 任务的接口函数
void ble_task(void* pvParameters);  // BLE 任务函数

#endif