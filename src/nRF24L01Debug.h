/*
 * MIT License
 *
 * Copyright (c) 2018 Erriez
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef NRF24L01_DEBUG_H__
#define NRF24L01_DEBUG_H__

#include <inttypes.h>

#include "nRF24L01_regs.h"

// RF modules support 10 MHz SPI bus speed
#ifndef RF24_SPI_SPEED
#define RF24_SPI_SPEED  10000000UL
#endif

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
