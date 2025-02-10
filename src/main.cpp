#include <Arduino.h>
#include <SPI.h>
#include <ACAN2517FD.h>
#include "pins.h"

SPIClass SPI2(FSPI);

ACAN2517FD can2(SELECT, SPI2, 255);

void setup()
{
  pinMode(8, OUTPUT);
  digitalWrite(8, 0);
  Serial.begin(115200);

  SPI2.begin(SOFT_SCK_PIN, SOFT_MISO_PIN, SOFT_MOSI_PIN);
  SPI2.setFrequency(20000000);

  ACAN2517FDSettings settings(ACAN2517FDSettings::OSC_40MHz, 1000 * 1000, DataBitRateFactor::x8);
  settings.mRequestedMode = ACAN2517FDSettings::NormalFD;

  const uint32_t errorCode = can2.begin(settings, NULL);
  Serial.println(errorCode);
}

static unsigned gSendDate = 0;
static unsigned gSentCount = 0;
static unsigned gReceivedCount = 0;

void loop()
{
  CANFDMessage message;
  if (gSendDate < millis())
  {
    message.id = 0x542;
    const bool ok = can2.tryToSend(message);
    if (ok)
    {
      gSendDate += 2000;
      gSentCount += 1;
      Serial.print("Sent: ");
      Serial.println(gSentCount);
    }
  }
  if (can2.receive(message))
  {
    gReceivedCount += 1;
    Serial.print("Received: ");
    Serial.println(gReceivedCount);
  }
}
// END FILE