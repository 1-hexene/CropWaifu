#include <ble_tasks.h>

// BLE 全局变量
BLEServer* pServer = nullptr;
BLECharacteristic* pCharacteristic = nullptr;
extern SemaphoreHandle_t bleUpdateSignal;
bool deviceConnected = false;
CropWaifuSensors cropWaifuSensors = CropWaifuSensors();

// 回调类：处理连接状态
class MyServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) override {
    deviceConnected = true;
  }

  void onDisconnect(BLEServer* pServer) override {
    deviceConnected = false;
  }
};

// BLE 初始化函数
canwaifu_status cropwaifu_ble_init() {
    Serial.println("[BLE.] Initializing BLE...");
  
    BLEDevice::init("CropWaifu"); // 初始化 BLE 设备名称
    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks());

    BLEService *pService = pServer->createService(SERVICE_UUID);

    pCharacteristic = pService->createCharacteristic(
                        CHARACTERISTIC_UUID,
                        BLECharacteristic::PROPERTY_READ |
                        BLECharacteristic::PROPERTY_NOTIFY
                    );

    pCharacteristic->setValue("114514");
    pService->start();

    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->start();
    Serial.println("[BLE.] Broadcasting BLE service...");
    return CANWAIFU_OK; // 返回成功状态
}

// BLE 任务函数
void ble_task(void* pvParameters) {

    Serial.println("[BLE.] BLE Task started");
    while (true) {
        if (deviceConnected) {
            // 如果设备已连接，检查是否有更新信号
            if (xSemaphoreTake(bleUpdateSignal, portMAX_DELAY) == pdTRUE) {
                // 发送通知
                pCharacteristic->setValue("Update from CropWaifu");
                pCharacteristic->notify();
                Serial.println("[BLE.] Notification sent");
            }
        }
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}
