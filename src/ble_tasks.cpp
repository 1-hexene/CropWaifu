#include <telecom/ble_tasks.h>
#include "BLEDevice.h" // Ensure BLEDevice.h is included for BLEAdvertising

// BLE 全局变量
BLEServer* pServer = nullptr;
BLECharacteristic* pCharacteristic = nullptr;
extern SemaphoreHandle_t bleUpdateSignal;
bool deviceConnected = false;

extern CropWaifuSensors cropWaifuSensors; // From sensor_tasks.cpp
extern canwaifu_status globalStatus;


// 回调类：处理连接状态
class MyServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) override {
    deviceConnected = true;
    Serial.println("[BTLE] Device connected!"); // Add a log for connection
  }

  void onDisconnect(BLEServer* pServer) override {
    deviceConnected = false;
    Serial.println("[BTLE] Device disconnected! Restarting advertising..."); // Log disconnection
    // After a disconnect, restart advertising to allow new connections
    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    if (pAdvertising != nullptr) {
        pAdvertising->start();
        Serial.println("[BTLE] Advertising restarted.");
    } else {
        Serial.println("[BTLE] Error: pAdvertising is null after disconnect.");
        globalStatus = CANWAIFU_ERR;
    }
  }
};

// BLE 初始化函数
canwaifu_status cropwaifu_ble_init() {
    Serial.println("[BTLE] Initializing BLE...");

    BLEDevice::init("CropWaifu-1"); // 初始化 BLE 设备名称
    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks());

    BLEService *pService = pServer->createService(SERVICE_UUID);

    pCharacteristic = pService->createCharacteristic(
                        CHARACTERISTIC_UUID,
                        BLECharacteristic::PROPERTY_READ |
                        BLECharacteristic::PROPERTY_NOTIFY
                    );

    pCharacteristic->setValue("114514");
    pCharacteristic->addDescriptor(new BLE2902()); //enable notifications
    pService->start();

    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->start();
    Serial.println("[BTLE] Broadcasting BLE service...");
    return CANWAIFU_OK; // 返回成功状态
}

// 打包BLE通知数据，返回20字节数组
void pack_ble_notify_data(uint8_t* notifyData, const CropWaifuSensors& sensors) {
    memset(notifyData, 0, 20);
    // BoardID
    notifyData[0] = (uint8_t)BOARD_ID;
    // 温度（float转int16，*100）
    int16_t temp = (int16_t)(sensors.temperature * 100);
    notifyData[2] = temp & 0xFF;
    notifyData[1] = (temp >> 8) & 0xFF;
    // 光照强度
    uint16_t light = sensors.lightIntensity;
    notifyData[4] = light & 0xFF;
    notifyData[3] = (light >> 8) & 0xFF;
    // 风扇转速
    uint16_t fan = sensors.fanSpeedRPM;
    notifyData[6] = fan & 0xFF;
    notifyData[5] = (fan >> 8) & 0xFF;
    // Humidity
    uint16_t humidity = (uint16_t)(sensors.humidity * 100);
    notifyData[8] = humidity & 0xFF;
    notifyData[7] = (humidity >> 8) & 0xFF;
    // Fan PWM
    notifyData[9] = sensors.fanPWM & 0xFF; // Fan PWM
    // LED PWM
    notifyData[10] = sensors.ledPWM & 0xFF;
}

// BLE 任务函数
void ble_task(void* pvParameters) {
    Serial.println("[BTLE] BLE Task started");
    uint8_t notifyData[20] = {0};
    TickType_t lastWakeTime = xTaskGetTickCount();
    while (true) {
        if (deviceConnected) {
            // 1. 非阻塞检查信号，有信号就立即notify（可多次处理）
            // While loop to process all pending signals
            while (xSemaphoreTake(bleUpdateSignal, 0) == pdTRUE) {
                pack_ble_notify_data(notifyData, cropWaifuSensors);
                pCharacteristic->setValue(notifyData, 20);
                pCharacteristic->notify();
                //Serial.println("[BLE.] Notification sent (signal)"); // Uncomment for verbose logging
            }
            // 2. 定时notify (only if no signal was processed recently or for guaranteed updates)
            // You might want to re-evaluate if you need both signal-based and periodic notifications.
            // For robust handling, signal-based is preferred for responsiveness, periodic for general updates.
            // If many signals come in quickly, the periodic one might be redundant.
            // For now, keeping both as in original.
            pack_ble_notify_data(notifyData, cropWaifuSensors);
            pCharacteristic->setValue(notifyData, 20);
            pCharacteristic->notify();
            //Serial.println("[BLE.] Notification sent (periodic)"); // Uncomment for verbose logging
        } else {
            // If not connected, simply wait and don't try to notify
            // This prevents issues if pCharacteristic is not ready or connection is down.
            // The onDisconnect callback handles re-advertising.
            //Serial.println("[BTLE] Not connected, waiting..."); // Uncomment for verbose logging
        }
        vTaskDelayUntil(&lastWakeTime, 500 / portTICK_PERIOD_MS);
    }
}