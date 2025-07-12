#ifndef BH1750FVI_LIB_H
#define BH1750FVI_LIB_H

#include <Wire.h>
#include <canwaifu_base.h>

class BH1750Sensor {
public:
    BH1750Sensor(uint8_t address = 0x23, TwoWire &wire = Wire);

    bool begin();
    uint16_t read_lux();

private:
    uint8_t _address;
    TwoWire* _wire;

    static const uint8_t CMD_CONT_HIRES_MODE = 0x10;
};

#endif