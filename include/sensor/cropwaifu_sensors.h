#ifndef CROPWAIFU_SENSORS_H
#define CROPWAIFU_SENSORS_H

#include <canwaifu_base.h>
#include <sensor/bh1750fvi_lib.h>
#include <SHT85.h>

class CropWaifuSensors
{
    private:
        /* data */
    public:
        float temperature; // 温度
        float humidity; // 湿度
        uint16_t lightIntensity; // 光照强度
        uint16_t fanSpeedRPM; // 风扇速度

        CropWaifuSensors() 
            : temperature(0.0f), lightIntensity(0.0f), fanSpeedRPM(0) {
            // Constructor implementation
        }
        ~CropWaifuSensors() = default;

        String toString() {
            String result = "CropWaifuSensors(temperature: " + String(temperature) +
                            ", lightIntensity: " + String(lightIntensity) +
                            ", fanSpeedRPM: " + String(fanSpeedRPM) + 
                            ", humidity: " + String(humidity) + ")";
            return result;
        }
};

#endif