#include <mqtt_tasks.h>

static EspMQTTClient* mqttClient;
extern QueueHandle_t ctrlCmdQueue;
extern SemaphoreHandle_t mqttHeartBeatSignal;


void mqtt_init(void) {
    // Initialize MQTT client
    mqttClient = new EspMQTTClient(
        MQTT_SERVER_IP, // MQTT server IP
        MQTT_SERVER_PORT, // MQTT server port
        MQTT_CLIENT_NAME // MQTT client name
    );
    Serial.println("[MQTT] Initializing MQTT client...");
    Serial.printf("[MQTT] Server IP: %s, Port: %d, Client Name: %s\n", 
                    MQTT_SERVER_IP, MQTT_SERVER_PORT, MQTT_CLIENT_NAME);
    mqttClient->setOnConnectionEstablishedCallback(&onConnectionEstablished);
    mqttClient->enableDebuggingMessages(ENABLE_MQTT_DEBUG); // Enable debugging messages
}

void mqtt_loop_task(void *parameter) {
  while (true) {
    mqttClient->loop();         // MQTT loop
    vTaskDelay(10 / portTICK_PERIOD_MS);  // Avoid CPU hogging
  }
}

void onConnectionEstablished(){
    mqttClient->subscribe("cropwaifu/control", &mqtt_message_receive_callback);
    xTaskCreate (mqtt_heartbeat_task, "MqttHeartbeatTask", 4096, NULL, 1, NULL);
}

void mqtt_heartbeat_task(void *pvParameters)
{
    while (1)
    {
        if (xSemaphoreTake(mqttHeartBeatSignal, portMAX_DELAY) == pdTRUE){ // 得到计时器中断信号
            // 发送心跳包
            Serial.println("[MQTT] Sending heartbeat...");
            mqttClient->publish("cropwaifu/heartbeat", ("{\"BoardID\":" + std::to_string(BOARD_ID) + 
                                                        ",\"UpTime\":" + std::to_string(millis()/1000) + "}").c_str()); // 发送心跳包
        }
    }
}

void mqtt_message_receive_callback(const String &payload)  {
    uint32_t timeOnReceive = millis(); // Update the time when a message is received
    // Serial.println(payload);
    JsonDocument commandJson;
    DeserializationError error = deserializeJson(commandJson, payload);

    // Check if deserialization was successful
    if (error) {
        Serial.print(F("[MQTT] deserializeJson() failed: "));
        Serial.println(error.c_str());
        return;
    }

    // Check if the JSON document contains the expected keys
    if (commandJson["messageID"].isNull() || commandJson["timestamp"].isNull() ||
        commandJson["boardID"].isNull() || commandJson["mode"].isNull() ||
        commandJson["fan"].isNull() || commandJson["led"].isNull() ||
        commandJson["temperature"].isNull() || commandJson["lightIntensity"].isNull()){
        Serial.println("[MQTT] Received JSON document without expected keys");
        return;
    }

    // Check if the board ID matches
    Serial.println("[MQTT] Received valid JSON document");
    if (commandJson["boardID"].as<uint8_t>() != BOARD_ID) {
        Serial.printf("[MQTT] Received command for board ID %d, but this board ID is %d\n",
                        commandJson["boardID"].as<uint8_t>(), BOARD_ID);
        return;
    }

    // Create a new ControlCommand object with the received data
    ControlCommand* ctrlCmd = new ControlCommand(
        commandJson["mode"].as<uint8_t>(),
        commandJson["fan"].as<uint8_t>(),
        commandJson["led"].as<uint8_t>(),
        commandJson["temperature"].as<float_t>(),
        commandJson["lightIntensity"].as<float_t>()
    );

    // Send the control command to the queue
    JsonDocument respondJson;
    String respondStringBuffer;
    respondJson["messageID"] = commandJson["messageID"].as<uint8_t>();
    respondJson["boardID"] = BOARD_ID;
    
    respondJson["timestamp"] = commandJson["timestamp"].as<uint32_t>() + (millis() - timeOnReceive); // Update timestamp to current time

    if (xQueueSend(ctrlCmdQueue, &ctrlCmd, 0) != pdTRUE) {
        Serial.println("[MQTT] Failed to send control command to queue");
        delete ctrlCmd; // Clean up if sending to queue fails
        respondJson["status"] = "FAIL";
    } else {
        Serial.println("[MQTT] Control command sent to queue successfully");
        respondJson["status"] = "OK";
    }

    serializeJson(respondJson, respondStringBuffer);
    mqttClient->publish("cropwaifu/respond", respondStringBuffer.c_str()); // 发送响应包
}