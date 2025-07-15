#ifndef CONTROL_TASKS_H
#define CONTROL_TASKS_H

#include <canwaifu_base.h>

#define CTRL_MODE_ABS 1
#define CTRL_MODE_PID 0

class ControlCommand
{
private:
    /* data */
public:
    uint8_t _mode; // 模式
    uint8_t _fan; // 风扇状态
    uint8_t _led; // LED状态
    float _temperature; // 温度
    float _lightIntensity; // 光照强度
    
    ControlCommand(uint8_t mode, uint8_t fan, uint8_t led, float temperature, float lightIntensity)
        : _mode(mode), _fan(fan), _led(led), _temperature(temperature), _lightIntensity(lightIntensity) {}
    
    ControlCommand() : _mode(0), _fan(0), _led(0), _temperature(0.0f), _lightIntensity(0.0f) {}
    
    ~ControlCommand() = default;

    void print() {
        Serial.printf("ControlCommand(mode: %d, fan: %d, led: %d, temperature: %.2f, lightIntensity: %.2f)\n",
                      _mode, _fan, _led, _temperature, _lightIntensity);
    }
};


canwaifu_status control_init(void);
void control_task(void *pvParameters);

#endif // CONTROL_TASKS_H