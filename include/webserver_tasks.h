#ifndef WEBSERVER_TASKS_H
#define WEBSERVER_TASKS_H

#define HTTP_SERVER_PORT 80
#include <canwaifu_base.h>
#include <ESPAsyncWebServer.h> 
#include <SPIFFS.h>
#include <ArduinoJson.h>

canwaifu_status webserver_init();
void webserver_post_task(void* pvParameters);
void webserver_get_task(void* pvParameters);
void webServerOnData(AsyncWebServerRequest *request);

#endif