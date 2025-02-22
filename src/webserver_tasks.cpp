#include <webserver_tasks.h>

static AsyncWebServer webServer(HTTP_SERVER_PORT);
File loading_html;
String loading_html_content;

void webserver_init(){
    while (!SPIFFS.begin(true))
    {
        Serial.println("[Webserver] SPIFFS init failed. Retrying in 3 seconds..");
        vTaskDelay(1000/portTICK_PERIOD_MS);
    }
    Serial.println("[Webserver] SPIFFS OK.");


    loading_html = SPIFFS.open("/loading.html");
    loading_html_content = loading_html.readStringUntil('\0');
    loading_html.close();


    webServer.on("/", HTTP_GET, [](AsyncWebServerRequest* request) { 
	   Serial.print("[WebServer]: New request received:");  // for debugging 
	   Serial.println("GET /");        // for debugging 
	   request->send(200, "text/html", loading_html_content); 
    });

    

    webServer.begin();
    Serial.printf("[WebServer] WebServer is listening on %d...\n", 80);
}
