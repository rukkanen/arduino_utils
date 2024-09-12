#include "HY62252A.h"

// Constructor: Initialize with address, data, and control pins
HY62252A::HY62252A(uint8_t *addr_pins, uint8_t *data_pins, uint8_t ce_pin, uint8_t oe_pin, uint8_t we_pin)
{
  _addr_pins = addr_pins;
  _data_pins = data_pins;
  _ce_pin = ce_pin;
  _oe_pin = oe_pin;
  _we_pin = we_pin;
}

// Initialize the GPIO pins for address, data, and control lines
void HY62252A::begin()
{
  // Set address pins as output
  for (int i = 0; i < 15; i++)
  {
    pinMode(_addr_pins[i], OUTPUT);
  }

  // Set data pins as input initially (for reading)
  setDataBusMode(INPUT);

  // Set control pins as output
  pinMode(_ce_pin, OUTPUT);
  pinMode(_oe_pin, OUTPUT);
  pinMode(_we_pin, OUTPUT);

  // Set default states for control signals
  digitalWrite(_ce_pin, HIGH); // Chip disabled
  digitalWrite(_oe_pin, HIGH); // Output disabled
  digitalWrite(_we_pin, HIGH); // Write disabled
}

// Set the address on the address bus
void HY62252A::setAddress(uint16_t address)
{
  for (int i = 0; i < 15; i++)
  {
    digitalWrite(_addr_pins[i], (address >> i) & 1);
  }
}

// Set data bus as input or output
void HY62252A::setDataBusMode(bool mode)
{
  for (int i = 0; i < 8; i++)
  {
    pinMode(_data_pins[i], mode);
  }
}

// Write data to the data bus
void HY62252A::writeDataBus(uint8_t data)
{
  for (int i = 0; i < 8; i++)
  {
    digitalWrite(_data_pins[i], (data >> i) & 1);
  }
}

// Read data from the data bus
uint8_t HY62252A::readDataBus()
{
  uint8_t data = 0;
  for (int i = 0; i < 8; i++)
  {
    data |= (digitalRead(_data_pins[i]) << i);
  }
  return data;
}

// Write a byte to the SRAM
void HY62252A::writeByte(uint16_t address, uint8_t data)
{
  // Set address
  setAddress(address);

  // Set data pins as output
  setDataBusMode(OUTPUT);

  // Write data to the bus
  writeDataBus(data);

  // Enable chip and write operation
  digitalWrite(_ce_pin, LOW);
  digitalWrite(_we_pin, LOW);
  delayMicroseconds(1); // Small delay to ensure stable write

  // End write operation
  digitalWrite(_we_pin, HIGH);
  digitalWrite(_ce_pin, HIGH);
}

// Read a byte from the SRAM
uint8_t HY62252A::readByte(uint16_t address)
{
  // Set address
  setAddress(address);

  // Set data pins as input
  setDataBusMode(INPUT);

  // Enable chip and read operation
  digitalWrite(_ce_pin, LOW);
  digitalWrite(_oe_pin, LOW);
  delayMicroseconds(1); // Small delay to ensure stable read

  // Read data from the bus
  uint8_t data = readDataBus();

  // End read operation
  digitalWrite(_oe_pin, HIGH);
  digitalWrite(_ce_pin, HIGH);

  return data;
}
