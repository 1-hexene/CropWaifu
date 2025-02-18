#include "wifi_tasks.h"

static WiFiClass Wifi;

void wifi_init(){
    Wifi.mode(WIFI_AP);
    WiFi.softAPConfig(IPAddress(10,0,0,1), IPAddress(10,0,0,1), IPAddress(255,255,255,0), IPAddress(10,0,0,100));
    Wifi.softAP("CanWaifu", "canwaifu");
    Serial.println("[Wifi] Wifi is up.");
}