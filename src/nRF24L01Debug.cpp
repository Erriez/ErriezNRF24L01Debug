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

#include "SPI.h"

#include "nRF24L01Debug.h"

// Comment the macro below to disable bitfield prints
#define USE_BITFIELDS

typedef struct {
  uint8_t address;
  const char *name;
} nRF24L01Registers;

static const nRF24L01Registers regs[NUM_REGISTERS] = {
  {REG_CONFIG,      "CONFIG"},
  {REG_EN_AA,       "EN_AA"},
  {REG_EN_RXADDR,   "EN_RXADDR"},
  {REG_SETUP_AW,    "SETUP_AW"},
  {REG_SETUP_RETR,  "SETUP_RETR"},
  {REG_RF_CH,       "RF_CH"},
  {REG_RF_SETUP,    "RF_SETUP"},
  {REG_STATUS,      "NRF_STATUS"},
  {REG_OBSERVE_TX,  "OBSERVE_TX"},
  {REG_RPD,         "RPD"},
  {REG_RX_ADDR_P0,  "RX_ADDR_P0"},
  {REG_RX_ADDR_P1,  "RX_ADDR_P1"},
  {REG_RX_ADDR_P2,  "RX_ADDR_P2"},
  {REG_RX_ADDR_P3,  "RX_ADDR_P3"},
  {REG_RX_ADDR_P4,  "RX_ADDR_P4"},
  {REG_RX_ADDR_P5,  "RX_ADDR_P5"},
  {REG_TX_ADDR,     "TX_ADDR"},
  {REG_RX_PW_P0,    "RX_PW_P0"},
  {REG_RX_PW_P1,    "RX_PW_P1"},
  {REG_RX_PW_P2,    "RX_PW_P2"},
  {REG_RX_PW_P3,    "RX_PW_P3"},
  {REG_RX_PW_P4,    "RX_PW_P4"},
  {REG_RX_PW_P5,    "RX_PW_P5"},
  {REG_FIFO_STATUS, "FIFO_STATUS"},
  {REG_DYNPD,       "DYNPD"},
  {REG_FEATURE,     "FEATURE"},
};

// -----------------------------------------------------------------------------
// Public functions
// -----------------------------------------------------------------------------
nRF24L01Debug::nRF24L01Debug(uint16_t csnPin)
{
  _csnPin = csnPin;
  
  // Configure SPI chip-select pin
	pinMode(_csnPin, OUTPUT);

	// CSN high to disable SPI chip select
	digitalWrite(_csnPin, HIGH);

	// Initialize SPI
	SPI.begin();
}

void nRF24L01Debug::printRegister(uint8_t addr, bool printBitfields)
{
  uint8_t idx;
  
  for (idx = 0; idx < NUM_REGISTERS; idx++) {
    if (regs[idx].address == addr) {
      break;
    }
  }
  
  if (idx >= NUM_REGISTERS) {
    printf_P(PSTR("  Error: Register 0x%02x does not exist\r\n"), addr);
    return;
  }
  
  switch (regs[idx].address) {
    case REG_RX_ADDR_P0:
    case REG_RX_ADDR_P1:
    case REG_TX_ADDR:
      printAddressRegister(regs[idx].address, regs[idx].name);
      break;
    default:
      printRegister(regs[idx].address, regs[idx].name, printBitfields);
  }
}

void nRF24L01Debug::printAllRegisters(bool printBitfields)
{
  uint8_t idx;
  
  printf_P(PSTR("nRF24L01 registers:\r\n"));

  for (idx = 0; idx < NUM_REGISTERS; idx++) {
    printRegister(regs[idx].address, printBitfields);
  }
}

uint8_t nRF24L01Debug::readRegister(uint8_t reg, uint8_t* buf, uint8_t len)
{
  uint8_t status;

  beginTransaction();
  status = SPI.transfer(CMD_R_REGISTER | (REGISTER_MASK & reg));
  while (len-- ) {
    *buf++ = SPI.transfer(0xff);
  }
  endTransaction();

  return status;
}

uint8_t nRF24L01Debug::readRegister(uint8_t reg)
{
  uint8_t result;
  
  beginTransaction();
  SPI.transfer(CMD_R_REGISTER | (REGISTER_MASK & reg));
  result = SPI.transfer(0xff);
  endTransaction();

  return result;
}

// -----------------------------------------------------------------------------
// Private functions
// -----------------------------------------------------------------------------
void nRF24L01Debug::printRegister(uint8_t addr, const char *desc, 
                                        bool printBitfields)
{
  uint8_t regVal;
  int8_t i;
  
  regVal = readRegister(addr);
  
  printf_P(PSTR("    0x%02X: 0x%02X (%s)\r\n"), addr, regVal, desc);
  
#ifdef USE_BITFIELDS
  if (printBitfields) {
    switch (addr) {
      case REG_CONFIG:
        printf_P(PSTR("\tMASK_RX_DR[6]  = %d\r\n"), regVal & (1<<6) ? 1 : 0);
        printf_P(PSTR("\tMASK_TX_DS[5]  = %d\r\n"), regVal & (1<<5) ? 1 : 0);
        printf_P(PSTR("\tMASK_MAX_RT[4] = %d\r\n"), regVal & (1<<4) ? 1 : 0);
        printf_P(PSTR("\tEN_CRC[3]      = %d\r\n"), regVal & (1<<3) ? 1 : 0);
        printf_P(PSTR("\tCRCO[2]        = %d\r\n"), regVal & (1<<2) ? 1 : 0);
        printf_P(PSTR("\tPWR_UP[1]      = %d\r\n"), regVal & (1<<1) ? 1 : 0);
        printf_P(PSTR("\tPRIM_RX[0]     = %d\r\n"), regVal & (1<<0) ? 1 : 0);
        break;
      case REG_EN_AA:
        for (i = 5; i >= 0; i--) {
          printf_P(PSTR("\tENAA_P%d[%d]     = %d\r\n"), i, i, regVal & (1<<i) ? 1 : 0);
        }
        break;
      case REG_EN_RXADDR:
        for (i = 5; i >= 0; i--) {
          printf_P(PSTR("\tERX_P%d[%d]      = %d\r\n"), i, i, regVal & (1<<i) ? 1 : 0);
        }
        break;
      case REG_SETUP_AW:
        printf_P(PSTR("\tAW[1:0]        = %d ("), (regVal & 0x03));
        switch (readRegister(REG_SETUP_AW)) {
          case 1:
            printf_P(PSTR("3 bytes"));
            break;
          case 2:
            printf_P(PSTR("4 bytes"));
            break;
          case 3:
            printf_P(PSTR("5 bytes"));
            break;
          default:
            printf_P(PSTR("Invalid"));
        }
        printf_P(PSTR(")\r\n"));
        break;
      case REG_SETUP_RETR:
        printf_P(PSTR("\tARD[7:4]       = %d (%d us)\r\n"), (regVal >> 4),
                      ((regVal >> 4) + 1) * 250);
        printf_P(PSTR("\tARC[3:0]       = %d (%s)\r\n"), (regVal & 0x0F), 
                      (regVal & 0x0F) ? "retries" : "Retry disabled");
        break;
      case REG_RF_CH:
        printf_P(PSTR("\tRF_CH[6:0]     = %d\r\n"), regVal & 0x7F);
        break;
      case REG_RF_SETUP:
        printf_P(PSTR("\tCONT_WAVE[7]   = %d\r\n"), (regVal & (1<<7)) ? 1 : 0);
        printf_P(PSTR("\tRF_DR_LOW[5]   = %d\r\n"), (regVal & (1<<5)) ? 1 : 0);
        printf_P(PSTR("\tPLL_LOCK[4]    = %d\r\n"), (regVal & (1<<4)) ? 1 : 0);
        printf_P(PSTR("\tRF_DR_HIGH[3]  = %d\r\n"), (regVal & (1<<3)) ? 1 : 0);
        printf_P(PSTR("\tRF_PWR[2:1]    = %d ("), (regVal & 0x06) >> 1);
        switch ((regVal & 0x06) >> 1) {
          case 0:
            printf_P(PSTR("-18dBm=MIN"));
            break;
          case 1:
            printf_P(PSTR("-12dBm=MID"));
            break;
          case 2:
            printf_P(PSTR("-16dBm=HIGH"));
            break;
          case 3:
            printf_P(PSTR("0dBm=MAX"));
            break;
          default:
            printf_P(PSTR("Invalid"));
        }
        printf_P(PSTR(")\r\n"));
        break;
      case REG_STATUS:
        printf_P(PSTR("\tRX_DR[6]       = %d\r\n"), (regVal & (1<<6)) ? 1 : 0);
        printf_P(PSTR("\tTX_DS[5]       = %d\r\n"), (regVal & (1<<5)) ? 1 : 0);
        printf_P(PSTR("\tMAX_RT[4]      = %d\r\n"), (regVal & (1<<4)) ? 1 : 0);
        printf_P(PSTR("\tRX_P_NO[3:1]   = %d\r\n"), (regVal & 0x0E) >> 1);
        printf_P(PSTR("\tTX_FULL[0]     = %d\r\n"), (regVal & (1<<0)) ? 1 : 0);
        break;
      case REG_OBSERVE_TX:
        printf_P(PSTR("\tPLOS_CNT[7:4]  = %d\r\n"), (regVal & 0xF0) >> 4);
        printf_P(PSTR("\tARC_CNT[3:0]   = %d\r\n"), regVal & 0x0F);
        break;
      case REG_RPD:
        printf_P(PSTR("\tRPD[0]         = %d\r\n"), regVal & (1<<0));
        break;
      case REG_RX_PW_P0:
      case REG_RX_PW_P1:
      case REG_RX_PW_P2:
      case REG_RX_PW_P3:
      case REG_RX_PW_P4:
      case REG_RX_PW_P5:
        printf_P(PSTR("\tRX_PW_P%d[5:0] = %d\r\n"), (addr - REG_RX_PW_P0), (regVal & 0x1F));
        break;
      case REG_FIFO_STATUS:
        printf_P(PSTR("\tTX_REUSE[6]    = %d\r\n"), (regVal & (1<<6)) ? 1 : 0);
        printf_P(PSTR("\tTX_FULL[5]     = %d\r\n"), (regVal & (1<<5)) ? 1 : 0);
        printf_P(PSTR("\tTX_EMPTY[4]    = %d\r\n"), (regVal & (1<<4)) ? 1 : 0);
        printf_P(PSTR("\tRX_FULL[1]     = %d\r\n"), (regVal & (1<<1)) ? 1 : 0);
        printf_P(PSTR("\tRX_EMPTY[0]    = %d\r\n"), (regVal & (1<<0)) ? 1 : 0);
        break;
      case REG_DYNPD:
        for (i = 5; i >= 0; i--) {
          printf_P(PSTR("\tDPL_P%d[%d]      = %d\r\n"), i, i, regVal & (1<<i) ? 1 : 0);
        }
        break;
      case REG_FEATURE:
        printf_P(PSTR("\tEN_DPL[2]      = %d\r\n"), (regVal & (1<<2)) ? 1 : 0);
        printf_P(PSTR("\tEN_ACK_PAY[1]  = %d\r\n"), (regVal & (1<<1)) ? 1 : 0);
        printf_P(PSTR("\tEN_DYN_ACK[0]  = %d\r\n"), (regVal & (1<<0)) ? 1 : 0);
        break;
      default:
        break;
    }
  }
#endif // USE_BITFIELDS
}

void nRF24L01Debug::printAddressRegister(uint8_t addr, const char *desc)
{
    uint8_t addrw;
    uint8_t buffer[5];
    uint8_t i;

    // Read Address width register
    switch (readRegister(REG_SETUP_AW)) {
        case 1:
            addrw = 3;
            break;
        case 2:
            addrw = 4;
            break;
        case 3:
            addrw = 5;
            break;
        default:
            // Invalid register value
            return;
    }

    printf_P(PSTR("    0x%02X: "), addr);
    readRegister(addr, buffer, addrw);
    for (i = 0; i < addrw; i++) {
        printf_P(PSTR("%02X "), buffer[i]);
    }
    printf_P(PSTR("(%s)\r\n"), desc);
}

void nRF24L01Debug::beginTransaction()
{
  SPI.beginTransaction(SPISettings(RF24_SPI_SPEED, MSBFIRST, SPI_MODE0));

  digitalWrite(_csnPin, LOW);
  delayMicroseconds(1);
}

void nRF24L01Debug::endTransaction() 
{
  digitalWrite(_csnPin, HIGH);
  delayMicroseconds(1);

  SPI.endTransaction();
}

void nRF24L01Debug::csn(bool mode)
{
  digitalWrite(_csnPin, mode);
  delayMicroseconds(5);
}
