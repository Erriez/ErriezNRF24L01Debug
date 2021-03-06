# Arduino nRF24L01 debug library.

This library prints nRF24L01(+) registers to the first serial port which is 
useful for debug purposes.

This library is compatible with the Nordic nRF24L01 and nRF24L01+ SPI 
transceivers.

## Installation
1. Start the Arduino IDE.
2. Download the latest version from:  
   https://github.com/Erriez/ErriezNRF24L01Debug/archive/master.zip
3. Click Sketch | Include Library | Add .ZIP Library... and select this ZIP.
5. Run the example.

## Library dependencies
* git clone https://github.com/Erriez/ErriezNRF24L01Debug
* git clone https://github.com/Erriez/ErriezNRF24L01Iface
* git clone https://github.com/Erriez/ErriezPrintf

### Example

Examples | nRF24L01(+) Debug | [DumpRegisters](https://github.com/Erriez/ErriezNRF24L01Debug/blob/master/examples/DumpRegisters/DumpRegisters.ino)

```c++
#include <Arduino.h>
#include <nRF24L01Debug.h>
#include <printf.h>
  
// SPI chip-select pin
#define CSN_PIN     8
  
// SPI clock
#define SPI_CLOCK   1000000UL
  
// Initialize nRF24L01 diagnostics library
nRF24L01Debug nRF24Debug(SPI_CLOCK, CSN_PIN);
  
void setup() 
{
  // Initialize serial port
  Serial.begin(115200);
  Serial.println(F("nRF24L01(+) dump registers example"));
  
  // Initialize printf
  printfBegin();
  
  // Print all registers without bitfields after RF24 initialization
  nRF24Debug.printAllRegisters();
  
  // Print all nRF24L01 registers with bitfields after RF24 configuration
  nRF24Debug.printAllRegisters(true);
}
  
void loop() 
{
  // Print one single register without bitfields
  nRF24Debug.printRegister(REG_STATUS, false);
  
  delay(5000);
}
```

### Serial output
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
        AW[1:0]        = 3 (5 bytes)
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
        RF_PWR[2:1]    = 3 (0dBm=MAX)
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
        RX_FULL[1]     = 0
        RX_EMPTY[0]    = 1
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

## Usage
1. Add #include ```<nRF24L01Debug.h>``` to your application.
2. Add #include ```<printf.h>```
3. Add ```printfBegin()``` call to ```setup()```.
3. Initialize the nRF24L01 debug library with SPI clock speed and ```CSN``` pin.

**Print a single register:**
```c++
  printRegister(uint8_t registerAddress, bool printBitfields);
```

**Read a register:**
```c++
  uint8_t readRegister(uint8_t reg);
```

**Read an address register:**
```c++
  uint8_t readRegister(uint8_t reg, uint8_t* buf, uint8_t len);
```

**Print all registers with/without register bitfields in your application:**
```c++
  printAllRegisters(bool printBitfields);
```

A macro ```USE_BITFIELDS``` is enabled by default to print register bitfields. Disable
this macro in ```nRF24L01Debug.cpp``` to save flash and RAM.
