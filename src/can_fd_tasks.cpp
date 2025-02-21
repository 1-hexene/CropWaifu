#include <can_fd_tasks.h>

static unsigned gSendDate = 0;
static unsigned gSentCount = 0;
static unsigned gReceivedCount = 0;

static SPIClass SPI2(FSPI);
static ACAN2517FD can2(MCP2518FD_CHIP_SELECT, SPI2, 255);

template <typename T>
void print_can_fd_message(const T _hardwareSerial, CANFDMessage* canFdMessage, bool direction_is_send)
{
    if (direction_is_send)
    {
        _hardwareSerial->println("---------[MCP2518FD][Send]----------");
        _hardwareSerial->print("Index = ");
        _hardwareSerial->println(gSentCount);
    }
    else
    {
        _hardwareSerial->println("--------[MCP2518FD][Received]-------");
        _hardwareSerial->print("Index: ");
        _hardwareSerial->println(gReceivedCount);
    }

    _hardwareSerial->print("ID: ");
    _hardwareSerial->println(canFdMessage->id);

    _hardwareSerial->print("Length: ");
    _hardwareSerial->println(canFdMessage->len);

    _hardwareSerial->print("Extended Frame: ");
    _hardwareSerial->println(canFdMessage->ext ? "Yes" : "No");

    _hardwareSerial->print("Type: ");
    switch (canFdMessage->type)
    {
    case CANFDMessage::Type::CAN_DATA:
        _hardwareSerial->println("Can data frame");
        break;
    case CANFDMessage::Type::CAN_REMOTE:
        _hardwareSerial->println("Can remote frame");
        break;
    case CANFDMessage::Type::CANFD_NO_BIT_RATE_SWITCH:
        _hardwareSerial->println("Can frame without bitrate switch");
        break;
    case CANFDMessage::Type::CANFD_WITH_BIT_RATE_SWITCH:
        _hardwareSerial->println("Can frame with bitrate switch");
        break;
    default:
        _hardwareSerial->println("Error");
        break;
    }

    _hardwareSerial->print("Content: ");
    if (canFdMessage->len)
    {
        for (uint8_t i = 0; i < canFdMessage->len; i++)
        {
            if (canFdMessage->data[i] < 0x10)
            {
                _hardwareSerial->print("0");
            }
            _hardwareSerial->print(canFdMessage->data[i], HEX);
            _hardwareSerial->print(" ");
        }
        _hardwareSerial->println();
    }
    else
    {
        _hardwareSerial->println("Nothing.");
    }
}

void can_fd_init()
{
    pinMode(8, OUTPUT);
    digitalWrite(8, 0);
    Serial.begin(115200);

    SPI2.begin(SOFT_SCK_PIN, SOFT_MISO_PIN, SOFT_MOSI_PIN);
    SPI2.setFrequency(20000000);

    /* ACAN2517FDSettings settings(
        ACAN2517FDSettings::OSC_40MHz,
        1000 * 1000, // DesiredArbitrationBitRate
        DataBitRateFactor::x8); */
    
    ACAN2517FDSettings settings(
        ACAN2517FDSettings::OSC_40MHz,
        1000*1000, // DesiredArbitrationBitRate
        DataBitRateFactor::x1);
    settings.mRequestedMode = ACAN2517FDSettings::Normal20B;

    const uint32_t errorCode = can2.begin(settings, NULL);
    if (!errorCode)
    {
        Serial.println("[MCP2518FD][Init] OK.");
    }
    else
    {
        Serial.println(errorCode);
    }
}

void can_fd_send_task(void *pvParameters)
{
    CANFDMessage message;
    while (1)
    {
        message.id = 0x542;
        const bool ok = can2.tryToSend(message);
        if (ok)
        {
            gSentCount += 1;
            if (PRINT_CAN_MSGS)
            {
                print_can_fd_message(&Serial, &message, true);
            }
            
            
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void can_fd_receive_task(void *pvParameters)
{
    CANFDMessage message;
    while (1)
    {
        if (can2.receive(message))
        {
            gReceivedCount += 1;
            if (PRINT_CAN_MSGS)
            {
                print_can_fd_message(&Serial, &message, false);
            }
            
            
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}