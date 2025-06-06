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

    if (xSemaphoreTake(canMsgMutex, 10 / portTICK_PERIOD_MS) == pdTRUE)
    {
        //Serial.println("[Webserver] Now has the lock.");
        for (int i = 0; i < 63; i++)
        {
            CanMsgWrapper msgWrapper = getCanMsgWrapperList()[i];
            if (msgWrapper.getLastFrequency() == 0 || (msgWrapper.getID() == 0 && msgWrapper.getLength() == 0))
                break; // 查到嗝屁的报文就直接跳出循环，报文处理那边应该能解决好的, 如果这一条消息的频率为0，而且一秒前也是0，那后面的就不管了

            // CANFDMessage msg = msgWrapper.getContent();
            JsonObject obj = array.add<JsonObject>();
            obj["id"] = msgWrapper.getID();
            obj["len"] = msgWrapper.getLength();

            switch (msgWrapper.getType())
            {
            case CANFDMessage::CAN_DATA:
                obj["type"] = "标准帧";
                break;
            case CANFDMessage::CAN_REMOTE:
                obj["type"] = "远程帧";
                break;
            case CANFDMessage::Type::CANFD_NO_BIT_RATE_SWITCH:
                obj["type"] = "CANFD帧(固定比特率)";
                break;
            case CANFDMessage::Type::CANFD_WITH_BIT_RATE_SWITCH:
                obj["type"] = "CANFD帧(可变比特率)";
                break;
            default:
                obj["type"] = "UNKNOWN";
                break;
            }

            JsonArray data = obj["data"].to<JsonArray>();
            for (int j = 0; j < msgWrapper.getLength(); j++)
            {
                data.add(msgWrapper.getContent()[j]);
            }
            obj["frequency"] = msgWrapper.getLastFrequency(); // 返回上一次清零前的频率
        }
        xSemaphoreGive(canMsgMutex);
    }
    String json;
    serializeJson(array, json);
    request->send(200, "application/json", json);
}