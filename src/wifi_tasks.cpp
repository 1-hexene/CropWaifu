#include <wifi_tasks.h>

canwaifu_status wifi_init(void) {
    // Initialize WiFi connection
    WiFi.begin("F429_2.4G", "FF442299"); // Replace with your WiFi SSID and password

    // Wait for connection
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }

    Serial.println("Connected to WiFi");
    return CANWAIFU_OK;
}