#ifndef HY62252A_H
#define HY62252A_H

#include <Arduino.h>
#include "ShiftRegister74HC595.h"

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
