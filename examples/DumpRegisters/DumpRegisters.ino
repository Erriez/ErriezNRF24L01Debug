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

/*
 * This nRF24L01 debug example prints nRF24L01(+) registers which is useful
 * during development.
 *
 * Library dependencies:
 *   git clone https://github.com/Erriez/ArduinoLibraryNRF24L01Debug
 *   git clone https://github.com/Erriez/ArduinoLibraryNRF24L01Iface
 *   git clone https://github.com/Erriez/ArduinoLibraryPrintf
 */

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
