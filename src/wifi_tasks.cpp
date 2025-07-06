#include <wifi_tasks.h>

canwaifu_status wifi_init(void) {
    // Initialize WiFi connection
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD); // Replace with your WiFi SSID and password

    // Wait for connectionWIFI
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }

    Serial.println("Connected to WiFi");
    return CANWAIFU_OK;
}