//
// This nRF24L01 debug example prints nRF24L01 registers which is useful
// during development.
//
// NRF24 library must be installed (https://github.com/nRF24/RF24)
// 

#include <RF24.h>
#include <nRF24L01Debug.h>

// Pin defines
#define CE      7
#define CSN     8

// Example pipes transmit and receive
const uint64_t pipe_tx = 0x0F0A050A55LL;
const uint64_t pipe_rx = 0x0F0A050AAALL;

// Initialize RF24 radio library
RF24 radio(CE, CSN);

// Initialize nRF24L01 diagnostics library
nRF24L01Debug nRF24Debug(CSN);

void setup() 
{
  // Initialize serial port
  Serial.begin(115200);
  Serial.println(F("nRF24L01 diagnostics example"));

  // Start RF24 radio
  radio.begin();

  // Detect nRF24L01 chip
  while (1) {
    if (radio.isChipConnected()) {
        Serial.println(F("nRF24L01 found"));
        break;
      } else {
        Serial.println(F("Error: nRF24L01 not found"));
      }
    delay(2000);
  }

  // Print all registers without bitfields after RF24 initialization
  nRF24Debug.printAllRegisters();
  
  // Example radio configuration
  radio.stopListening();
  radio.setChannel(6);
  radio.setAddressWidth(5);
  radio.setPayloadSize(32);
  radio.openWritingPipe(pipe_tx);
  radio.openReadingPipe(1, pipe_rx);
  radio.closeReadingPipe(2);
  radio.closeReadingPipe(3);
  radio.closeReadingPipe(4);
  radio.closeReadingPipe(5);
  radio.setRetries(5, 15);
  radio.disableDynamicPayloads();
  radio.setAutoAck(false);
  radio.setPALevel(RF24_PA_HIGH);
  radio.setDataRate(RF24_1MBPS);
  radio.disableCRC();
  radio.startListening();

  // Print all nRF24L01 registers with bitfields after RF24 configuration
  nRF24Debug.printAllRegisters(true);
  
  Serial.println(F("nRF24L01 setup completed"));
}

void loop() 
{
  // Print one single register without bitfields
  nRF24Debug.printRegister(NRF_STATUS, false);
  
  delay(5000);
}
