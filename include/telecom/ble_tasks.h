#ifndef BLE_TAKS_H
#define BLE_TAKS_H

#include <canwaifu_base.h>
#include <sensor/cropwaifu_sensors.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#define SERVICE_UUID        "11451411-4514-1145-1411-000000000001"
#define CHARACTERISTIC_UUID "19198101-9198-1019-1981-000000000001"

canwaifu_status cropwaifu_ble_init();  // 创建 BLE 任务的接口函数
void ble_task(void* pvParameters);  // BLE 任务函数

#endif