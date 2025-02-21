#include <webserver_tasks.h>

static AsyncWebServer webServer(HTTP_SERVER_PORT);

void webserver_init(){
    
    webServer.on("/", HTTP_GET, [](AsyncWebServerRequest* request) { 
	   Serial.print("[WebServer]: New request received:");  // for debugging 
	   Serial.println("GET /");        // for debugging 
	   request->send(200, "text/html", "<html><body><h1> 404 Not Found </h1></body></html>"); 
    });
    webServer.begin();
    Serial.printf("[WebServer] WebServer is listening on %d...\n", 80);
}

void webserver_post_task(void* pvParameters){

}

void webserver_get_task(void* pvParameters){

}