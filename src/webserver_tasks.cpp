#include <webserver_tasks.h>

extern SemaphoreHandle_t canMsgMutex;

AsyncWebServer webServer(80);
String index_html_content;

canwaifu_status webserver_init()
{
    SPIFFS.begin(true);

    // 主文件接口
    webServer.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
                 { request->send(SPIFFS, "/index.html", String(), false); });

    // 版本号接口
    webServer.on("/version", HTTP_GET, [](AsyncWebServerRequest *request)
                 { request->send(200, "text/html", SW_VER); });

    // post方法接口
    // webServer.on("/post", HTTP_POST, [](AsyncWebServerRequest * request){});

    // Can报文数据接口
    webServer.on("/data", HTTP_GET, webServerOnData);

    webServer.begin();
    return CANWAIFU_OK;
}

void webServerOnData(AsyncWebServerRequest *request)
{

    JsonDocument doc;
    JsonArray array = doc.to<JsonArray>();

    String json;
    serializeJson(array, json);
    request->send(200, "application/json", json);
}