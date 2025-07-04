#include <mqtt_tasks.h>

static EspMQTTClient* mqttClient;
extern QueueHandle_t ctrlCmdQueue;


canwaifu_status mqtt_init(void) {
    // Initialize MQTT client
    mqttClient = new EspMQTTClient(
        "10.0.0.171", // MQTT server IP
        5001, // MQTT server port
        "CropWaifu-Test" // MQTT client name
    );
    mqttClient->setOnConnectionEstablishedCallback(onConnectionEstablished);
    mqttClient->enableDebuggingMessages(true); // Enable debugging messages
    
    return mqttClient->isConnected() ? CANWAIFU_OK : CANWAIFU_ERR; // Connect to MQTT server
}

void mqtt_loop_task(void *parameter) {
  while (true) {
    mqttClient->loop();         // MQTT loop
    vTaskDelay(10 / portTICK_PERIOD_MS);  // Avoid CPU hogging
  }
}

void onConnectionEstablished() {

  mqttClient->subscribe("cropwaifu/test", [] (const String &payload)  {
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
    if (xQueueSend(ctrlCmdQueue, &ctrlCmd, 0) != pdTRUE) {
        Serial.println("[MQTT] Failed to send control command to queue");
        delete ctrlCmd; // Clean up if sending to queue fails
    } else {
        Serial.println("[MQTT] Control command sent to queue successfully");
    }
});

  mqttClient->publish("cropwaifu/test", "This is a message");
}