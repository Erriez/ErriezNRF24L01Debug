//
// https://github.com/Erriez/nRF24L01Debug
//

#include "SPI.h"

#include <nRF24L01Debug.h>

// Comment the macro below to disable bitfield prints
#define USE_BITFIELDS

#define NUM_REGISTERS   26

typedef struct {
  uint8_t address;
  const char *name;
} nRF24L01Registers;

static const nRF24L01Registers regs[NUM_REGISTERS] = {
  {NRF_CONFIG, "CONFIG"},
  {EN_AA,      "EN_AA"},
  {EN_RXADDR,  "EN_RXADDR"},
  {SETUP_AW,   "SETUP_AW"},
  {SETUP_RETR, "SETUP_RETR"},
  {RF_CH,      "RF_CH"},
  {RF_SETUP,   "RF_SETUP"},
  {NRF_STATUS, "NRF_STATUS"},
  {OBSERVE_TX, "OBSERVE_TX"},
  {CD,         "CD/RPD"},
  {RX_ADDR_P0, "RX_ADDR_P0"},
  {RX_ADDR_P1, "RX_ADDR_P1"},
  {RX_ADDR_P2, "RX_ADDR_P2"},
  {RX_ADDR_P3, "RX_ADDR_P3"},
  {RX_ADDR_P4, "RX_ADDR_P4"},
  {RX_ADDR_P5, "RX_ADDR_P5"},
  {TX_ADDR,    "TX_ADDR"},
  {RX_PW_P0,   "RX_PW_P0"},
  {RX_PW_P1,   "RX_PW_P1"},
  {RX_PW_P2,   "RX_PW_P2"},
  {RX_PW_P3,   "RX_PW_P3"},
  {RX_PW_P4,   "RX_PW_P4"},
  {RX_PW_P5,   "RX_PW_P5"},
  {FIFO_STATUS, "FIFO_STATUS"},
  {DYNPD,      "DYNPD"},
  {FEATURE,    "FEATURE"},
};

// -----------------------------------------------------------------------------
// Public functions
// -----------------------------------------------------------------------------
nRF24L01Debug::nRF24L01Debug(uint16_t csnPin)
{
  _csnPin = csnPin;
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
    case RX_ADDR_P0:
    case RX_ADDR_P1:
    case TX_ADDR:
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
  status = _SPI.transfer(R_REGISTER | (REGISTER_MASK & reg));
  while ( len-- ){
    *buf++ = _SPI.transfer(0xff);
  }
  endTransaction();

  return status;
}

uint8_t nRF24L01Debug::readRegister(uint8_t reg)
{
  uint8_t result;
  
  beginTransaction();
  _SPI.transfer(R_REGISTER | (REGISTER_MASK & reg));
  result = _SPI.transfer(0xff);
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
      case NRF_CONFIG:
        printf_P(PSTR("\tMASK_RX_DR[6]  = %d\r\n"), regVal & (1<<6) ? 1 : 0);
        printf_P(PSTR("\tMASK_TX_DS[5]  = %d\r\n"), regVal & (1<<5) ? 1 : 0);
        printf_P(PSTR("\tMASK_MAX_RT[4] = %d\r\n"), regVal & (1<<4) ? 1 : 0);
        printf_P(PSTR("\tEN_CRC[3]      = %d\r\n"), regVal & (1<<3) ? 1 : 0);
        printf_P(PSTR("\tCRCO[2]        = %d\r\n"), regVal & (1<<2) ? 1 : 0);
        printf_P(PSTR("\tPWR_UP[1]      = %d\r\n"), regVal & (1<<1) ? 1 : 0);
        printf_P(PSTR("\tPRIM_RX[0]     = %d\r\n"), regVal & (1<<0) ? 1 : 0);
        break;
      case EN_AA:
        for (i = 5; i >= 0; i--) {
          printf_P(PSTR("\tENAA_P%d[%d]     = %d\r\n"), i, i, regVal & (1<<i) ? 1 : 0);
        }
        break;
      case EN_RXADDR:
        for (i = 5; i >= 0; i--) {
          printf_P(PSTR("\tERX_P%d[%d]      = %d\r\n"), i, i, regVal & (1<<i) ? 1 : 0);
        }
        break;
      case SETUP_AW:
        printf_P(PSTR("\tAW[1:0]        = %d"), (regVal & 0x03));
        switch (readRegister(SETUP_AW)) {
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
        printf_P(PSTR("\r\n"));
        break;
      case SETUP_RETR:
        printf_P(PSTR("\tARD[7:4]       = %d (%d us)\r\n"), (regVal >> 4),
                      ((regVal >> 4) + 1) * 250);
        printf_P(PSTR("\tARC[3:0]       = %d (%s)\r\n"), (regVal & 0x0F), 
                      (regVal & 0x0F) ? "retries" : "Retry disabled");
        break;
      case RF_CH:
        printf_P(PSTR("\tRF_CH[6:0]     = %d\r\n"), regVal & 0x7F);
        break;
      case RF_SETUP:
        printf_P(PSTR("\tCONT_WAVE[7]   = %d\r\n"), (regVal & (1<<7)) ? 1 : 0);
        printf_P(PSTR("\tRF_DR_LOW[5]   = %d\r\n"), (regVal & (1<<5)) ? 1 : 0);
        printf_P(PSTR("\tPLL_LOCK[4]    = %d\r\n"), (regVal & (1<<4)) ? 1 : 0);
        printf_P(PSTR("\tRF_DR_HIGH[3]  = %d\r\n"), (regVal & (1<<3)) ? 1 : 0);
        printf_P(PSTR("\tRF_PWR[2:1]    = %d ("), (regVal & 0x06) >> 1);
        switch ((regVal & 0x06) >> 1) {
          case 0:
            printf_P(PSTR("-18dBm (MIN)"));
            break;
          case 1:
            printf_P(PSTR("-12dBm (MID)"));
            break;
          case 2:
            printf_P(PSTR("-16dBm (HIGH)"));
            break;
          case 3:
            printf_P(PSTR("0dBm (MAX)"));
            break;
        }
        printf_P(PSTR(")\r\n"));
        break;
      case NRF_STATUS:
        printf_P(PSTR("\tRX_DR[6]       = %d\r\n"), (regVal & (1<<6)) ? 1 : 0);
        printf_P(PSTR("\tTX_DS[5]       = %d\r\n"), (regVal & (1<<5)) ? 1 : 0);
        printf_P(PSTR("\tMAX_RT[4]      = %d\r\n"), (regVal & (1<<4)) ? 1 : 0);
        printf_P(PSTR("\tRX_P_NO[3:1]   = %d\r\n"), (regVal & 0x0E) >> 1);
        printf_P(PSTR("\tTX_FULL[0]     = %d\r\n"), (regVal & (1<<0)) ? 1 : 0);
        break;
      case OBSERVE_TX:
        printf_P(PSTR("\tPLOS_CNT[7:4]  = %d\r\n"), (regVal & 0xF0) >> 4);
        printf_P(PSTR("\tARC_CNT[3:0]   = %d\r\n"), regVal & 0x0F);
        break;
      case CD:
        printf_P(PSTR("\tRPD[0]         = %d\r\n"), regVal & (1<<0));
        break;
      case RX_PW_P0:
      case RX_PW_P1:
      case RX_PW_P2:
      case RX_PW_P3:
      case RX_PW_P4:
      case RX_PW_P5:
        printf_P(PSTR("\tRX_PW_P%d[5:0] = %d\r\n"), (addr - RX_PW_P0), (regVal & 0x1F));
        break;
      case FIFO_STATUS:
        printf_P(PSTR("\tTX_REUSE[6]    = %d\r\n"), (regVal & (1<<6)) ? 1 : 0);
        printf_P(PSTR("\tTX_FULL[5]     = %d\r\n"), (regVal & (1<<5)) ? 1 : 0);
        printf_P(PSTR("\tTX_EMPTY[4]    = %d\r\n"), (regVal & (1<<4)) ? 1 : 0);
        printf_P(PSTR("\tTX_REUSE[2]    = %d\r\n"), (regVal & (1<<2)) ? 1 : 0);
        printf_P(PSTR("\tTX_FULL[1]     = %d\r\n"), (regVal & (1<<1)) ? 1 : 0);
        printf_P(PSTR("\tTX_EMPTY[0]    = %d\r\n"), (regVal & (1<<0)) ? 1 : 0);
        break;
      case DYNPD:
        for (i = 5; i >= 0; i--) {
          printf_P(PSTR("\tDPL_P%d[%d]      = %d\r\n"), i, i, regVal & (1<<i) ? 1 : 0);
        }
        break;
      case FEATURE:
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
    switch (readRegister(SETUP_AW)) {
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

inline void nRF24L01Debug::beginTransaction() 
{
  _SPI.beginTransaction(SPISettings(RF24_SPI_SPEED, MSBFIRST, SPI_MODE0));
  csn(LOW);
}

inline void nRF24L01Debug::endTransaction() 
{
  csn(HIGH);
  _SPI.endTransaction();
}

void nRF24L01Debug::csn(bool mode)
{
  digitalWrite(_csnPin, mode);
  delayMicroseconds(5);
}
