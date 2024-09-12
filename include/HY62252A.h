#ifndef HY62252A_H
#define HY62252A_H

#include <Arduino.h>

class HY62252A
{
public:
  // Constructor: Provide pin numbers for address, data, and control lines
  HY62252A(uint8_t *addr_pins, uint8_t *data_pins, uint8_t ce_pin, uint8_t oe_pin, uint8_t we_pin);

  // Initialize GPIOs
  void begin();

  // Write a byte to SRAM at a specified address
  void writeByte(uint16_t address, uint8_t data);

  // Read a byte from SRAM from a specified address
  uint8_t readByte(uint16_t address);

private:
  // Address and data pin arrays
  uint8_t *_addr_pins;
  uint8_t *_data_pins;

  // Control signal pins
  uint8_t _ce_pin;
  uint8_t _oe_pin;
  uint8_t _we_pin;

  // Set address on the address bus
  void setAddress(uint16_t address);

  // Set data bus as input or output
  void setDataBusMode(bool mode);

  // Read/write from data bus
  void writeDataBus(uint8_t data);
  uint8_t readDataBus();
};

#endif
