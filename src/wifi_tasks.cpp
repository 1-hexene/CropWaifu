#include <telecom/wifi_tasks.h>

canwaifu_status wifi_init(void) {
    // Initialize WiFi connection
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD); // Replace with your WiFi SSID and password

    // Wait for connectionWIFI
    unsigned long wifi_timeout = millis() + 10000; // Set timeout to 10 seconds
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("[WIFI] Connecting to WiFi...");
        if (millis() > wifi_timeout) { // Timeout after 10 seconds
            Serial.println("[WIFI] Failed to connect to WiFi after 10 seconds.");
            return CANWAIFU_ERR; // Return error status
        }
    }

    Serial.println("[WIFI] Connected to WiFi");
    Serial.println("=========[STATUS]=========");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    Serial.print("RSSI: ");
    Serial.print(WiFi.RSSI());
    Serial.println(" dBm");
    Serial.print("BSSID: ");
    Serial.println(WiFi.BSSIDstr());
    Serial.println("==========================");
    return CANWAIFU_OK;
}