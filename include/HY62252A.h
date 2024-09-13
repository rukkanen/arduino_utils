#ifndef HY62252A_H
#define HY62252A_H

#include <Arduino.h>
#include "ShiftRegister74HC595.h"

/*
Examples

uint8_t data[] = {0x10, 0x20, 0x30, 0x40}; // Example data
sram.writeBlock(0x0000, data, sizeof(data)); // Write data block to SRAM

uint8_t buffer[4]; // Buffer to store read data
sram.readBlock(0x0000, buffer, sizeof(buffer)); // Read data back into buffer

Key-value _should_ work like this:
// store 16 chars value with 4 chars key
const char* key = "ABCD";     // 4-character key
const char* value = "1234567890abcdef";  // 16-character value
sram.storeKeyValue(0x0000, key, value);  // Store the key-value pair at address 0x0000

// fetch 16 char value based on a address
char keyBuffer[5];     // Buffer to hold the retrieved key (4 chars + null terminator)
char valueBuffer[17];  // Buffer to hold the retrieved value (16 chars + null terminator)
sram.getKeyValue(0x0000, keyBuffer, valueBuffer);  // Retrieve key-value pair from address 0x0000

// find key and get value
char valueBuffer[17];  // Buffer to hold the 16-character value (plus null terminator)
if (sram.getValueFor("ABCD", valueBuffer, 0x0000, 0x1000)) {
    Serial.print("Value found: ");
    Serial.println(valueBuffer);
} else {
    Serial.println("Key not found");
}

*/

class HY62252A
{
public:
  // Constructor without shift register
  HY62252A(uint8_t *addr_pins, uint8_t *data_pins, uint8_t ce_pin, uint8_t oe_pin, uint8_t we_pin);

  // Constructor with shift register for address pins
  HY62252A(ShiftRegister74HC595 *shiftRegister, uint8_t *data_pins, uint8_t ce_pin, uint8_t oe_pin, uint8_t we_pin, uint8_t addr_bits_in_shift_register = 8);

  // Initialize GPIOs and/or shift register
  void begin();

  // Write a byte to SRAM at a specified address
  void writeByte(uint16_t address, uint8_t data);

  // Read a byte from SRAM from a specified address
  uint8_t readByte(uint16_t address);

  // Write a block of data to SRAM starting at a given address
  void writeBlock(uint16_t startAddress, const uint8_t *data, uint16_t length);

  // Read a block of data from SRAM starting at a given address
  void readBlock(uint16_t startAddress, uint8_t *buffer, uint16_t length);

  // Key-value operations
  void storeKeyValue(uint16_t startAddress, const char *key, const char *value);
  void getKeyValue(uint16_t startAddress, char *keyBuffer, char *valueBuffer);
  int16_t findKey(const char *keyToFind, uint16_t startAddress, uint16_t endAddress);
  bool getValueFor(const char *keyToFind, char *valueBuffer, uint16_t startAddress, uint16_t endAddress);

private:
  // Address and data pin arrays (for direct GPIO usage)
  uint8_t *_addr_pins = nullptr;
  uint8_t *_data_pins;

  // Control signal pins
  uint8_t _ce_pin;
  uint8_t _oe_pin;
  uint8_t _we_pin;

  // Optional Shift Register for address lines
  ShiftRegister74HC595 *_shiftRegister = nullptr;
  uint8_t _addr_bits_in_shift_register;

  // Set address on the address bus (GPIO or shift register)
  void setAddress(uint16_t address);

  // Set data bus as input or output
  void setDataBusMode(bool mode);

  // Write/read data from the data bus
  void writeDataBus(uint8_t data);
  uint8_t readDataBus();
};

#endif
