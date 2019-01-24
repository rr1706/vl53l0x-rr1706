#include <SoftVL53L1X.h>

SoftVL53L1X::SoftVL53L1X(SoftwareWire* wire) {
  VL53L1X();
  _theWire = wire;
}

void SoftVL53L1X::writeReg(uint16_t reg, uint8_t value)
{
  _theWire->beginTransmission(getAddress());
  _theWire->write((reg >> 8) & 0xFF); // reg high byte
  _theWire->write( reg       & 0xFF); // reg low byte
  _theWire->write(value);
  last_status = _theWire->endTransmission(true);
} 

// Write a 16-bit register
void SoftVL53L1X::writeReg16Bit(uint16_t reg, uint16_t value)
{ 
  _theWire->beginTransmission(getAddress());
  _theWire->write((reg >> 8) & 0xFF); // reg high byte
  _theWire->write( reg       & 0xFF); // reg low byte
  _theWire->write((value >> 8) & 0xFF); // value high byte
  _theWire->write( value       & 0xFF); // value low byte
  last_status = _theWire->endTransmission(true);
}

// Write a 32-bit register
void SoftVL53L1X::writeReg32Bit(uint16_t reg, uint32_t value)
{ 
  _theWire->beginTransmission(getAddress());
  _theWire->write((reg >> 8) & 0xFF); // reg high byte
  _theWire->write( reg       & 0xFF); // reg low byte
  _theWire->write((value >> 24) & 0xFF); // value highest byte
  _theWire->write((value >> 16) & 0xFF);
  _theWire->write((value >>  8) & 0xFF); 
  _theWire->write( value        & 0xFF); // value lowest byte
  last_status = _theWire->endTransmission(true);
}

// Read an 8-bit register
uint8_t SoftVL53L1X::readReg(regAddr reg)
{ 
  uint8_t value;
  
  _theWire->beginTransmission(getAddress());
  _theWire->write((reg >> 8) & 0xFF); // reg high byte
  _theWire->write( reg       & 0xFF); // reg low byte
  last_status = _theWire->endTransmission(true);
  
  _theWire->requestFrom(getAddress(), (uint8_t)1);
  value = _theWire->read();
  
  //Serial.print("readReg("); Serial.print(reg, HEX); Serial.print(") = "); Serial.println(value, HEX);
  return value;
}

// Read a 16-bit register
uint16_t SoftVL53L1X::readReg16Bit(uint16_t reg)
{
  uint16_t value;

  _theWire->beginTransmission(getAddress());
  _theWire->write((reg >> 8) & 0xFF); // reg high byte
  _theWire->write( reg       & 0xFF); // reg low byte
  last_status = _theWire->endTransmission(true);

  _theWire->requestFrom(getAddress(), (uint8_t)2);
  value  = (uint16_t)_theWire->read() << 8; // value high byte
  value |=           _theWire->read();      // value low byte

  //Serial.print("readReg16Bit("); Serial.print(reg, HEX); Serial.print(") = "); Serial.println(value, HEX);
  return value;
} 
  
// Read a 32-bit register
uint32_t SoftVL53L1X::readReg32Bit(uint16_t reg)
{ 
  uint32_t value;
  
  _theWire->beginTransmission(getAddress());
  _theWire->write((reg >> 8) & 0xFF); // reg high byte
  _theWire->write( reg       & 0xFF); // reg low byte
  last_status = _theWire->endTransmission(true);

  _theWire->requestFrom(getAddress(), (uint8_t)4);
  value  = (uint32_t)_theWire->read() << 24; // value highest byte
  value |= (uint32_t)_theWire->read() << 16;
  value |= (uint16_t)_theWire->read() <<  8;
  value |=           _theWire->read();       // value lowest byte

  //Serial.print("readReg32Bit("); Serial.print(reg, HEX); Serial.print(") = "); Serial.println(value, HEX);
  return value;
}

bool SoftVL53L1X::init(bool io_2v8) {
  return VL53L1X::init(io_2v8);
}

/*
// Write an arbitrary number of bytes from the given array to the sensor,
// starting at the given register 
void SoftVL53L1X::writeMulti(uint8_t reg, uint8_t const * src, uint8_t count) {   
  _theWire->beginTransmission(getAddress());
  _theWire->write(reg); 
    
  while (count-- > 0) { 
    _theWire->write(*(src++));
  } 

  last_status = _theWire->endTransmission(true); 
}
  
// Read an arbitrary number of bytes from the sensor, starting at the given
// register, into the given array
void SoftVL53L1X::readMulti(uint8_t reg, uint8_t * dst, uint8_t count) {
  _theWire->beginTransmission(getAddress());
  _theWire->write(reg);
  last_status = _theWire->endTransmission(true);
  
  _theWire->requestFrom(getAddress(), count);
  
  while (count-- > 0)
  { 
    *(dst++) = _theWire->read();
  }
} 
*/
// read measurement results into buffer
void SoftVL53L1X::readResults()
{
  _theWire->beginTransmission(getAddress());
  _theWire->write((byte)((RESULT__RANGE_STATUS >> 8) & 0xFF)); // reg high byte
  _theWire->write( RESULT__RANGE_STATUS       & 0xFF); // reg low byte
  last_status = _theWire->endTransmission(true);

  _theWire->requestFrom(getAddress(), (uint8_t)17);

  results.range_status = _theWire->read();

  _theWire->read(); // report_status: not used

  results.stream_count = _theWire->read();

  results.dss_actual_effective_spads_sd0  = (uint16_t)_theWire->read() << 8; // high byte
  results.dss_actual_effective_spads_sd0 |=           _theWire->read();      // low byte

  _theWire->read(); // peak_signal_count_rate_mcps_sd0: not used
  _theWire->read();

  results.ambient_count_rate_mcps_sd0  = (uint16_t)_theWire->read() << 8; // high byte
  results.ambient_count_rate_mcps_sd0 |=           _theWire->read();      // low byte

  _theWire->read(); // sigma_sd0: not used
  _theWire->read();

  _theWire->read(); // phase_sd0: not used
  _theWire->read();

  results.final_crosstalk_corrected_range_mm_sd0  = (uint16_t)_theWire->read() << 8; // high byte
  results.final_crosstalk_corrected_range_mm_sd0 |=           _theWire->read();      // low byte

  results.peak_signal_count_rate_crosstalk_corrected_mcps_sd0  = (uint16_t)_theWire->read() << 8; // high byte
  results.peak_signal_count_rate_crosstalk_corrected_mcps_sd0 |=           _theWire->read();      // low byte
}

