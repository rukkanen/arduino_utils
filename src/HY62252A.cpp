#include "HY62252A.h"

// Constructor: Initialize with direct GPIO control for address and data pins
HY62252A::HY62252A(uint8_t *addr_pins, uint8_t *data_pins, uint8_t ce_pin, uint8_t oe_pin, uint8_t we_pin)
    : _addr_pins(addr_pins), _data_pins(data_pins), _ce_pin(ce_pin), _oe_pin(oe_pin), _we_pin(we_pin)
{
  _shiftRegister = nullptr; // Not using shift register
}

// Constructor: Initialize with shift register for address pins
HY62252A::HY62252A(ShiftRegister74HC595 *shiftRegister, uint8_t *data_pins, uint8_t ce_pin, uint8_t oe_pin, uint8_t we_pin, uint8_t addr_bits_in_shift_register)
    : _shiftRegister(shiftRegister), _data_pins(data_pins), _ce_pin(ce_pin), _oe_pin(oe_pin), _we_pin(we_pin), _addr_bits_in_shift_register(addr_bits_in_shift_register)
{
  _addr_pins = nullptr; // Address pins will be handled by shift register
}

// Initialize GPIO pins or shift register
void HY62252A::begin()
{
  if (_addr_pins)
  {
    // If using direct GPIO control, set address pins as outputs
    for (int i = 0; i < 15; i++)
    {
      pinMode(_addr_pins[i], OUTPUT);
    }
  }

  // Set data pins as inputs initially (for reading)
  setDataBusMode(INPUT);

  // Set control pins as output
  pinMode(_ce_pin, OUTPUT);
  pinMode(_oe_pin, OUTPUT);
  pinMode(_we_pin, OUTPUT);

  // Set default states for control signals
  digitalWrite(_ce_pin, HIGH); // Chip disabled
  digitalWrite(_oe_pin, HIGH); // Output disabled
  digitalWrite(_we_pin, HIGH); // Write disabled

  if (_shiftRegister)
  {
    // Initialize the shift register if used
    _shiftRegister->clearAll();
    _shiftRegister->updateRegisters();
  }
}

// Set the address on the address bus (either GPIO or shift register)
void HY62252A::setAddress(uint16_t address)
{
  if (_shiftRegister)
  {
    // Use shift register for the first N address bits
    for (uint8_t i = 0; i < _addr_bits_in_shift_register; i++)
    {
      _shiftRegister->setPin(i, (address >> i) & 1);
    }
    _shiftRegister->updateRegisters();
  }

  if (_addr_pins)
  {
    // Use direct GPIO control for the remaining address bits
    for (int i = 0; i < 15; i++)
    {
      digitalWrite(_addr_pins[i], (address >> i) & 1);
    }
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
  // Set the address
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
  // Set the address
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

void HY62252A::writeBlock(uint16_t startAddress, const uint8_t *data, uint16_t length)
{
  for (uint16_t i = 0; i < length; i++)
  {
    writeByte(startAddress + i, data[i]);
  }
}

void HY62252A::readBlock(uint16_t startAddress, uint8_t *buffer, uint16_t length)
{
  for (uint16_t i = 0; i < length; i++)
  {
    buffer[i] = readByte(startAddress + i);
  }
}

// Store a key-value pair at a specified SRAM address
void HY62252A::storeKeyValue(uint16_t startAddress, const char *key, const char *value)
{
  // Store the 4-byte key at the startAddress
  writeBlock(startAddress, (uint8_t *)key, 4);

  // Store the 16-byte value immediately after the key
  writeBlock(startAddress + 4, (uint8_t *)value, 16);
}

// Retrieve a key-value pair from a specified SRAM address
void HY62252A::getKeyValue(uint16_t startAddress, char *keyBuffer, char *valueBuffer)
{
  // Read the 4-byte key
  readBlock(startAddress, (uint8_t *)keyBuffer, 4);

  // Read the 16-byte value
  readBlock(startAddress + 4, (uint8_t *)valueBuffer, 16);
}

// Search for a key and return its address or -1 if not found
int16_t HY62252A::findKey(const char *keyToFind, uint16_t startAddress, uint16_t endAddress)
{
  char keyBuffer[5] = {0}; // Buffer to hold the key read from SRAM

  for (uint16_t addr = startAddress; addr < endAddress; addr += 20)
  { // 20 bytes per key-value pair
    // Read the 4-byte key from memory
    readBlock(addr, (uint8_t *)keyBuffer, 4);

    // Compare the read key with the key we're searching for
    if (strncmp(keyBuffer, keyToFind, 4) == 0)
    {
      return addr; // Key found, return the address
    }
  }

  return -1; // Return -1 if the key is not found
}
