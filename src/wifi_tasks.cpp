#include <wifi_tasks.h>

static WiFiClass Wifi;

canwaifu_status wifi_init(){
    Wifi.mode(WIFI_AP);
    WiFi.softAPConfig(IPAddress(10,0,0,1), IPAddress(10,0,0,1), IPAddress(255,255,255,0), IPAddress(10,0,0,100));
    WiFi.softAPsetHostname("CanWaifu");
    uint8_t fuseSerialNumber[6] = {1,1,4,5,1,4};
    std::string ssid = "CanWaifu_";
    // 获取熔丝烧录的硬件唯一id
    esp_efuse_mac_get_default(fuseSerialNumber);
    ssid.append((char*)fuseSerialNumber);

    if (Wifi.softAP("CanWaifu", "canwaifu")){
        Serial.println("[Wifi] Wifi is up.");
        return CANWAIFU_OK;
    } else
    {
        Serial.println("[Wifi] Wifi setup error!");
        return CANWAIFU_ERR;
    }
}
