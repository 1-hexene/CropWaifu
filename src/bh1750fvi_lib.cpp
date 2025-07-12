#include <sensor/bh1750fvi_lib.h>


BH1750Sensor::BH1750Sensor(uint8_t address, TwoWire &wire)
    : _address(address), _wire(&wire) {}

bool BH1750Sensor::begin() {
    _wire->beginTransmission(_address);
    _wire->write(CMD_CONT_HIRES_MODE);
    return (_wire->endTransmission() == 0);
}

uint16_t BH1750Sensor::read_lux() {
    _wire->requestFrom(_address, (uint8_t)2);
    if (_wire->available() == 2) {
        uint16_t raw = (_wire->read() << 8) | _wire->read();
        return (uint16_t) (raw / 1.2); // 1 lx resolution
    } else {
        return -1; // error
    }
}
