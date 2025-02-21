#ifndef WEBSERVER_TASKS_H
#define WEBSERVER_TASKS_H

#define HTTP_SERVER_PORT 80
#include <ESPAsyncWebServer.h> 
#include <SPIFFS.h>
#include <iostream>
#include <fstream>
#include <string>

void webserver_init();
void webserver_post_task(void* pvParameters);
void webserver_get_task(void* pvParameters);

#endif