#Arduino nRF24L01 debug library.

This library is an addition to the RF24 library to read and print nRF24L01 
registers debug purposes. This library is compatbile with the Nordic nRF24L01 
and nRF24L01+ SPI transceivers.

## Usage
1. Add #include <nRF24L01Debug.h> to your application.
2. Initialize the nRF24L01 debug library with the CSN pin.
3. Print all registers with/without register bitfields in your application:
```
  printAllRegisters(bool printBitfields);
```

4. Print a single register:
```
  printRegister(uint8_t registerAddress, bool printBitfields);
```

5. Read a register:
```
  uint8_t readRegister(uint8_t reg);
```

6. Read an address register:
```
  uint8_t readRegister(uint8_t reg, uint8_t* buf, uint8_t len);
```

A macro ```USE_BITFIELDS``` is enabled by default to print register bitfields. Disable
this macro in ```nRF24L01Debug.cpp``` to save flash and RAM.

### Example
```
// 1. Add include files:
#include <RF24.h>
#include <nRF24L01Debug.h> 
  
// Initialize RF24 radio library
RF24 radio(CE_PIN, CSN_PIN);
  
// 2. Initialize nRF24L01 debug library:
nRF24L01Debug nRF24Debug(CSN_PIN);
  
void setup() 
{
  // Initialize serial port
  Serial.begin(115200);
  
  // Start RF24 radio
  radio.begin();
  
  // Initialize the radio
  // ...
  
  // 3. Print all nRF24L01 registers with bitfields after RF24 configuration
  nRF24Debug.printAllRegisters(true);
  
  // 4. Print one single register without bitfields
  nRF24Debug.printRegister(NRF_STATUS, false);
}
```

### Output
```
nRF24L01 registers:
    0x00: 0x0E (CONFIG)
    0x01: 0x3F (EN_AA)
    0x02: 0x03 (EN_RXADDR)
    0x03: 0x03 (SETUP_AW)
    0x04: 0x5F (SETUP_RETR)
    0x05: 0x4C (RF_CH)
    0x06: 0x07 (RF_SETUP)
    0x07: 0x0E (NRF_STATUS)
    0x08: 0x00 (OBSERVE_TX)
    0x09: 0x00 (CD/RPD)
    0x0A: E7 E7 E7 E7 E7 (RX_ADDR_P0)
    0x0B: C2 C2 C2 C2 C2 (RX_ADDR_P1)
    0x0C: 0xC3 (RX_ADDR_P2)
    0x0D: 0xC4 (RX_ADDR_P3)
    0x0E: 0xC5 (RX_ADDR_P4)
    0x0F: 0xC6 (RX_ADDR_P5)
    0x10: E7 E7 E7 E7 E7 (TX_ADDR)
    0x11: 0x00 (RX_PW_P0)
    0x12: 0x00 (RX_PW_P1)
    0x13: 0x00 (RX_PW_P2)
    0x14: 0x00 (RX_PW_P3)
    0x15: 0x00 (RX_PW_P4)
    0x16: 0x00 (RX_PW_P5)
    0x17: 0x11 (FIFO_STATUS)
    0x1C: 0x00 (DYNPD)
    0x1D: 0x00 (FEATURE)
  
nRF24L01 registers:
    0x00: 0x0E (CONFIG)
        MASK_RX_DR[6]  = 0
        MASK_TX_DS[5]  = 0
        MASK_MAX_RT[4] = 0
        EN_CRC[3]      = 1
        CRCO[2]        = 1
        PWR_UP[1]      = 1
        PRIM_RX[0]     = 0
    0x01: 0x3F (EN_AA)
        ENAA_P5[5]     = 1
        ENAA_P4[4]     = 1
        ENAA_P3[3]     = 1
        ENAA_P2[2]     = 1
        ENAA_P1[1]     = 1
        ENAA_P0[0]     = 1
    0x02: 0x03 (EN_RXADDR)
        ERX_P5[5]      = 0
        ERX_P4[4]      = 0
        ERX_P3[3]      = 0
        ERX_P2[2]      = 0
        ERX_P1[1]      = 1
        ERX_P0[0]      = 1
    0x03: 0x03 (SETUP_AW)
        AW[1:0]        = 35 bytes
    0x04: 0x5F (SETUP_RETR)
        ARD[7:4]       = 5 (1500 us)
        ARC[3:0]       = 15 (retries)
    0x05: 0x4C (RF_CH)
        RF_CH[6:0]     = 76
    0x06: 0x07 (RF_SETUP)
        CONT_WAVE[7]   = 0
        RF_DR_LOW[5]   = 0
        PLL_LOCK[4]    = 0
        RF_DR_HIGH[3]  = 0
        RF_PWR[2:1]    = 3 (0dBm (MAX))
    0x07: 0x0E (NRF_STATUS)
        RX_DR[6]       = 0
        TX_DS[5]       = 0
        MAX_RT[4]      = 0
        RX_P_NO[3:1]   = 7
        TX_FULL[0]     = 0
    0x08: 0x00 (OBSERVE_TX)
        PLOS_CNT[7:4]  = 0
        ARC_CNT[3:0]   = 0
    0x09: 0x00 (CD/RPD)
        RPD[0]         = 0
    0x0A: E7 E7 E7 E7 E7 (RX_ADDR_P0)
    0x0B: C2 C2 C2 C2 C2 (RX_ADDR_P1)
    0x0C: 0xC3 (RX_ADDR_P2)
    0x0D: 0xC4 (RX_ADDR_P3)
    0x0E: 0xC5 (RX_ADDR_P4)
    0x0F: 0xC6 (RX_ADDR_P5)
    0x10: E7 E7 E7 E7 E7 (TX_ADDR)
    0x11: 0x00 (RX_PW_P0)
        RX_PW_P0[5:0] = 0
    0x12: 0x00 (RX_PW_P1)
        RX_PW_P1[5:0] = 0
    0x13: 0x00 (RX_PW_P2)
        RX_PW_P2[5:0] = 0
    0x14: 0x00 (RX_PW_P3)
        RX_PW_P3[5:0] = 0
    0x15: 0x00 (RX_PW_P4)
        RX_PW_P4[5:0] = 0
    0x16: 0x00 (RX_PW_P5)
        RX_PW_P5[5:0] = 0
    0x17: 0x11 (FIFO_STATUS)
        TX_REUSE[6]    = 0
        TX_FULL[5]     = 0
        TX_EMPTY[4]    = 1
        TX_REUSE[2]    = 0
        TX_FULL[1]     = 0
        TX_EMPTY[0]    = 1
    0x1C: 0x00 (DYNPD)
        DPL_P5[5]      = 0
        DPL_P4[4]      = 0
        DPL_P3[3]      = 0
        DPL_P2[2]      = 0
        DPL_P1[1]      = 0
        DPL_P0[0]      = 0
    0x1D: 0x00 (FEATURE)
        EN_DPL[2]      = 0
        EN_ACK_PAY[1]  = 0
        EN_DYN_ACK[0]  = 0
```