#ifndef BLE_TAKS_H
#define BLE_TAKS_H

#include <canwaifu_base.h>
#include <cropwaifu_sensors.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

canwaifu_status cropwaifu_ble_init();  // 创建 BLE 任务的接口函数
void ble_task(void* pvParameters);  // BLE 任务函数

#endif