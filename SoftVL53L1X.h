#ifndef SoftVL53L1X_h
#define SoftVL53L1X_h

#include <Arduino.h>
#include <VL53L1X.h>
#include <SoftwareWire.h>

class SoftVL53L1X:public VL53L1X {
  public:
    SoftVL53L1X(SoftwareWire* wire);
    void writeReg(uint16_t reg, uint8_t value);
    void writeReg16Bit(uint16_t reg, uint16_t value);
    void writeReg32Bit(uint16_t reg, uint32_t value);
    uint8_t readReg(regAddr reg);
    uint16_t readReg16Bit(uint16_t reg);
    uint32_t readReg32Bit(uint16_t reg);
    bool init(bool io_2v8 = true); 
    void readResults();

  protected:
    SoftwareWire* _theWire;
};

#endif
