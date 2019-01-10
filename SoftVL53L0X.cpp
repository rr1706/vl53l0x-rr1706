#include <SoftVL53L0X.h>

SoftVL53L0X::SoftVL53L0X(SoftwareWire* wire) {
  _theWire = wire;
}

void SoftVL53L0X::writeReg(uint8_t reg, uint8_t value)
{
  _theWire->beginTransmission(getAddress());
  _theWire->write(reg);
  _theWire->write(value);
  last_status = _theWire->endTransmission(true);
} 

// Write a 16-bit register
void SoftVL53L0X::writeReg16Bit(uint8_t reg, uint16_t value)
{ 
  _theWire->beginTransmission(getAddress());
  _theWire->write(reg);
  _theWire->write((value >> 8) & 0xFF); // value high byte
  _theWire->write( value       & 0xFF); // value low byte
  last_status = _theWire->endTransmission(true);
}

// Write a 32-bit register
void SoftVL53L0X::writeReg32Bit(uint8_t reg, uint32_t value)
{ 
  _theWire->beginTransmission(getAddress());
  _theWire->write(reg);
  _theWire->write((value >> 24) & 0xFF); // value highest byte
  _theWire->write((value >> 16) & 0xFF);
  _theWire->write((value >>  8) & 0xFF); 
  _theWire->write( value        & 0xFF); // value lowest byte
  last_status = _theWire->endTransmission(true);
}

// Read an 8-bit register
uint8_t SoftVL53L0X::readReg(uint8_t reg)
{ 
  uint8_t value;
  
  _theWire->beginTransmission(getAddress());
  _theWire->write(reg);
  last_status = _theWire->endTransmission(true);
  
  _theWire->requestFrom(getAddress(), (uint8_t)1);
  value = _theWire->read();
  
  return value;
}

// Read a 16-bit register
uint16_t SoftVL53L0X::readReg16Bit(uint8_t reg)
{
  uint16_t value;

  _theWire->beginTransmission(getAddress());
  _theWire->write(reg);
  last_status = _theWire->endTransmission(true);

  _theWire->requestFrom(getAddress(), (uint8_t)2);
  value  = (uint16_t)_theWire->read() << 8; // value high byte
  value |=           _theWire->read();      // value low byte

  return value;
} 
  
// Read a 32-bit register
uint32_t SoftVL53L0X::readReg32Bit(uint8_t reg)
{ 
  uint32_t value;
  
  _theWire->beginTransmission(getAddress());
  _theWire->write(reg);
  last_status = _theWire->endTransmission(true);

  _theWire->requestFrom(getAddress(), (uint8_t)4);
  value  = (uint32_t)_theWire->read() << 24; // value highest byte
  value |= (uint32_t)_theWire->read() << 16;
  value |= (uint16_t)_theWire->read() <<  8;
  value |=           _theWire->read();       // value lowest byte

  return value;
}

bool SoftVL53L0X::init(bool io_2v8) {
  VL53L0X::init(io_2v8);
}

// Write an arbitrary number of bytes from the given array to the sensor,
// starting at the given register 
void SoftVL53L0X::writeMulti(uint8_t reg, uint8_t const * src, uint8_t count) {   
  _theWire->beginTransmission(getAddress());
  _theWire->write(reg); 
    
  while (count-- > 0) { 
    _theWire->write(*(src++));
  } 

  last_status = _theWire->endTransmission(true); 
}
  
// Read an arbitrary number of bytes from the sensor, starting at the given
// register, into the given array
void SoftVL53L0X::readMulti(uint8_t reg, uint8_t * dst, uint8_t count) {
  _theWire->beginTransmission(getAddress());
  _theWire->write(reg);
  last_status = _theWire->endTransmission(true);
  
  _theWire->requestFrom(getAddress(), count);
  
  while (count-- > 0)
  { 
    *(dst++) = _theWire->read();
  }
} 

