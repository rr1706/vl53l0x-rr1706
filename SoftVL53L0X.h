#ifndef SoftVL53L0X_h
#define SoftVL53L0X_h

#include <Arduino.h>
#include <VL53L0X.h>
#include <SoftwareWire.h>

class SoftVL53L0X:public VL53L0X {
  public:
    SoftVL53L0X(SoftwareWire* wire);
    void writeReg(uint8_t reg, uint8_t value);
    void writeReg16Bit(uint8_t reg, uint16_t value);
    void writeReg32Bit(uint8_t reg, uint32_t value);
    uint8_t readReg(uint8_t reg);
    uint16_t readReg16Bit(uint8_t reg);
    uint32_t readReg32Bit(uint8_t reg);
    bool init(bool io_2v8 = true); 
    void writeMulti(uint8_t reg, uint8_t const * src, uint8_t count);
    void readMulti(uint8_t reg, uint8_t * dst, uint8_t count);

  protected:
    SoftwareWire* _theWire;
};

#endif
