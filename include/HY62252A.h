#ifndef HY62252A_H
#define HY62252A_H

#include <Arduino.h>
#include "ShiftRegister74HC595.h"

/**
 * Class to interface with the HY62252A SRAM chip (32K x 8).
 * Supports direct GPIO control or two 74HC595 shift registers for address lines.
 */
class HY62252A
{
public:
  // Constructor for direct GPIO control of address and data lines.
  HY62252A(uint8_t *addr_pins, uint8_t *data_pins, uint8_t ce_pin, uint8_t oe_pin, uint8_t we_pin);

  // Constructor for using two shift registers to control the address lines.
  HY62252A(ShiftRegister74HC595 *shiftRegister1, ShiftRegister74HC595 *shiftRegister2, uint8_t *data_pins,
           uint8_t ce_pin, uint8_t oe_pin, uint8_t we_pin, uint8_t addr_bits_in_shift_register1, uint8_t addr_bits_in_shift_register2);

  // Initialize the SRAM and set up control pins.
  void begin();

  // Write a byte to a specified address in the SRAM.
  void writeByte(uint16_t address, uint8_t data);

  // Read a byte from a specified address in the SRAM.
  uint8_t readByte(uint16_t address);

  // Write a block of data starting at a specified address.
  void writeBlock(uint16_t startAddress, const uint8_t *data, uint16_t length);

  // Read a block of data starting at a specified address.
  void readBlock(uint16_t startAddress, uint8_t *buffer, uint16_t length);

  // Store a key-value pair at a specified SRAM address.
  void storeKeyValue(uint16_t startAddress, const char *key, const char *value);

  // Get the value for a given key from SRAM.
  bool getValueForKey(const char *keyToFind, char *valueBuffer, uint16_t startAddress, uint16_t endAddress);

private:
  // Set the address on the address bus using GPIO or shift registers.
  void setAddress(uint16_t address);

  // Set the data bus to input or output mode.
  void setDataBusMode(uint8_t mode);

  // Write data to the data bus.
  void writeDataBus(uint8_t data);

  // Read data from the data bus.
  uint8_t readDataBus();

  uint8_t *_addr_pins;                   // Address pins (if using GPIO)
  uint8_t *_data_pins;                   // Data pins (I/O1-I/O8)
  uint8_t _ce_pin, _oe_pin, _we_pin;     // Control pins for CE, OE, and WE
  ShiftRegister74HC595 *_shiftRegister1; // First shift register (lower address bits)
  ShiftRegister74HC595 *_shiftRegister2; // Second shift register (upper address bits)
  uint8_t _addr_bits_in_shift_register1; // Number of address bits controlled by shiftRegister1
  uint8_t _addr_bits_in_shift_register2; // Number of address bits controlled by shiftRegister2
};

#endif
