//
// https://github.com/Erriez/nRF24L01Debug
//

#ifndef NRF24L01_DEBUG_H__
#define NRF24L01_DEBUG_H__

#include <Arduino.h>
#include <RF24_config.h>
#include <nRF24L01.h> 

class nRF24L01Debug 
{
public:
  nRF24L01Debug(uint16_t csnPin);
  
  void printRegister(uint8_t addr, bool printBitfields=false);
  void printAllRegisters(bool printBitfields=false);
  uint8_t readRegister(uint8_t reg, uint8_t* buf, uint8_t len);
  uint8_t readRegister(uint8_t reg);

private:
  uint16_t _csnPin;

  void printRegister(uint8_t addr, const char *desc, bool printBitfields);
  void printAddressRegister(uint8_t addr, const char *desc);
  void csn(bool mode);
  inline void beginTransaction();
  inline void endTransaction();
};

#endif // NRF24L01_DEBUG_H__
