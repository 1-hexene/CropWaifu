#include <wifi_tasks.h>

static WiFiClass Wifi;

/* 查表法转换字符串
*/
static std::string fuseSnToSsid(const uint8_t* data, uint8_t len, bool uppercase = true) {
    const char* hex_chars = uppercase ? "0123456789ABCDEF" : "0123456789abcdef";
    std::string ssid = "CanWaifu_";;
    ssid.reserve(len * 2); // 预分配空间优化性能
    
    //避免SSID太长，就用最后三个字节
    for (size_t i = len/2; i < len; ++i) {
        uint8_t byte = data[i];
        ssid.push_back(hex_chars[(byte >> 4) & 0x0F]); // 处理高4位
        ssid.push_back(hex_chars[byte & 0x0F]);         // 处理低4位
    }

    return ssid;

}

canwaifu_status wifi_init(){
    Wifi.mode(WIFI_AP);
    WiFi.softAPConfig(IPAddress(10,0,0,1), IPAddress(10,0,0,1), IPAddress(255,255,255,0), IPAddress(10,0,0,100));
    WiFi.softAPsetHostname("CanWaifu");
    uint8_t fuseSerialNumber[6] = {};

    // 获取熔丝烧录的硬件唯一id
    esp_efuse_mac_get_default(fuseSerialNumber);
    std::string ssid = fuseSnToSsid(fuseSerialNumber, sizeof(fuseSerialNumber));
    Serial.print("[Wifi] SSID is: ");
    Serial.println(ssid.c_str());

    if (Wifi.softAP(ssid.c_str(), "canwaifu")){
        Serial.println("[Wifi] Wifi is up.");
        return CANWAIFU_OK;
    } else
    {
        Serial.println("[Wifi] Wifi setup error!");
        return CANWAIFU_ERR;
    }
}

