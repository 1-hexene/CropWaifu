#ifndef CROPWAIFU_SENSORS_H
#define CROPWAIFU_SENSORS_H

#include <canwaifu_base.h>

class CropWaifuSensors
{
    private:
        /* data */
    public:
        float temperature; // 温度
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
                            ", fanSpeedRPM: " + String(fanSpeedRPM) + ")";
            return result;
        }
};

#endif