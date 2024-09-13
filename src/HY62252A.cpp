#include "HY62252A.h"
#include "logger.h"

/**
 * Constructor for direct GPIO control of address and data lines.
 *
 * @param addr_pins Array of GPIO pins for address bus (A0-A14).
 * @param data_pins Array of GPIO pins for data bus (I/O1-I/O8).
 * @param ce_pin Chip Enable control pin.
 * @param oe_pin Output Enable control pin.
 * @param we_pin Write Enable control pin.
 */
HY62252A::HY62252A(uint8_t *addr_pins, uint8_t *data_pins, uint8_t ce_pin, uint8_t oe_pin, uint8_t we_pin)
    : _addr_pins(addr_pins), _data_pins(data_pins), _ce_pin(ce_pin), _oe_pin(oe_pin), _we_pin(we_pin)
{
  _shiftRegister1 = nullptr;
  _shiftRegister2 = nullptr;
}

/**
 * Constructor for using two shift registers to control the address lines.
 *
 * @param shiftRegister1 Pointer to the first shift register for lower address bits.
 * @param shiftRegister2 Pointer to the second shift register for upper address bits.
 * @param data_pins Array of GPIO pins for data bus (I/O1-I/O8).
 * @param ce_pin Chip Enable control pin.
 * @param oe_pin Output Enable control pin.
 * @param we_pin Write Enable control pin.
 * @param addr_bits_in_shift_register1 Number of address bits controlled by shiftRegister1.
 * @param addr_bits_in_shift_register2 Number of address bits controlled by shiftRegister2.
 */
HY62252A::HY62252A(ShiftRegister74HC595 *shiftRegister1, ShiftRegister74HC595 *shiftRegister2, uint8_t *data_pins,
                   uint8_t ce_pin, uint8_t oe_pin, uint8_t we_pin, uint8_t addr_bits_in_shift_register1, uint8_t addr_bits_in_shift_register2)
    : _shiftRegister1(shiftRegister1), _shiftRegister2(shiftRegister2), _data_pins(data_pins),
      _ce_pin(ce_pin), _oe_pin(oe_pin), _we_pin(we_pin), _addr_bits_in_shift_register1(addr_bits_in_shift_register1), _addr_bits_in_shift_register2(addr_bits_in_shift_register2)
{
  _addr_pins = nullptr; // Address pins handled by shift registers
}

/**
 * Initializes the SRAM, setting up control pins and data pins.
 * If using shift registers, initializes them as well.
 */
void HY62252A::begin()
{
  if (_addr_pins)
  {
    // Initialize address pins if using GPIO
    for (int i = 0; i < 15; i++)
    {
      pinMode(_addr_pins[i], OUTPUT);
    }
  }

  // Initialize data pins (start as inputs)
  setDataBusMode(INPUT);

  // Initialize control pins
  pinMode(_ce_pin, OUTPUT);
  pinMode(_oe_pin, OUTPUT);
  pinMode(_we_pin, OUTPUT);

  // Default state: chip disabled, output disabled, write disabled
  digitalWrite(_ce_pin, HIGH);
  digitalWrite(_oe_pin, HIGH);
  digitalWrite(_we_pin, HIGH);

  // Initialize shift registers if used
  if (_shiftRegister1)
  {
    _shiftRegister1->clearAll();
    _shiftRegister1->updateRegisters();
  }
  if (_shiftRegister2)
  {
    _shiftRegister2->clearAll();
    _shiftRegister2->updateRegisters();
  }
}

/**
 * Sets the address on the SRAM, either using direct GPIO control or shift registers.
 *
 * @param address The 16-bit address to set on the address bus.
 */
void HY62252A::setAddress(uint16_t address)
{
  // Use shift registers for address lines
  if (_shiftRegister1)
  {
    for (uint8_t i = 0; i < _addr_bits_in_shift_register1; i++)
    {
      _shiftRegister1->setPin(i, (address >> i) & 1);
    }
    _shiftRegister1->updateRegisters();
  }

  if (_shiftRegister2)
  {
    for (uint8_t i = 0; i < _addr_bits_in_shift_register2; i++)
    {
      _shiftRegister2->setPin(i, (address >> (_addr_bits_in_shift_register1 + i)) & 1);
    }
    _shiftRegister2->updateRegisters();
  }

  // Use direct GPIO if no shift registers
  if (_addr_pins)
  {
    for (int i = 0; i < 15; i++)
    {
      digitalWrite(_addr_pins[i], (address >> i) & 1);
    }
  }
}

/**
 * Sets the data bus to input or output mode.
 *
 * @param mode Set as INPUT or OUTPUT.
 */
void HY62252A::setDataBusMode(bool mode)
{
  for (int i = 0; i < 8; i++)
  {
    pinMode(_data_pins[i], mode);
  }
}

/**
 * Writes data to the data bus.
 *
 * @param data The byte to write to the bus.
 */
void HY62252A::writeDataBus(uint8_t data)
{
  for (int i = 0; i < 8; i++)
  {
    digitalWrite(_data_pins[i], (data >> i) & 1);
  }
}

/**
 * Reads data from the data bus.
 *
 * @return The byte read from the bus.
 */
uint8_t HY62252A::readDataBus()
{
  uint8_t data = 0;
  for (int i = 0; i < 8; i++)
  {
    data |= (digitalRead(_data_pins[i]) << i);
  }
  return data;
}

/**
 * Writes a single byte to the SRAM at the specified address.
 *
 * @param address The address to write to.
 * @param data The byte to write.
 */
void HY62252A::writeByte(uint16_t address, uint8_t data)
{
  setAddress(address);
  setDataBusMode(OUTPUT);
  writeDataBus(data);

  digitalWrite(_ce_pin, LOW);
  digitalWrite(_we_pin, LOW);
  delayMicroseconds(1);
  digitalWrite(_we_pin, HIGH);
  digitalWrite(_ce_pin, HIGH);
}

/**
 * Reads a single byte from the SRAM at the specified address.
 *
 * @param address The address to read from.
 * @return The byte read.
 */
uint8_t HY62252A::readByte(uint16_t address)
{
  setAddress(address);
  setDataBusMode(INPUT);

  digitalWrite(_ce_pin, LOW);
  digitalWrite(_oe_pin, LOW);
  delayMicroseconds(1);
  uint8_t data = readDataBus();
  digitalWrite(_oe_pin, HIGH);
  digitalWrite(_ce_pin, HIGH);

  return data;
}

/**
 * Writes a block of data to SRAM starting from the specified address.
 *
 * @param startAddress The start address.
 * @param data Pointer to the data to write.
 * @param length Number of bytes to write.
 */
void HY62252A::writeBlock(uint16_t startAddress, const uint8_t *data, uint16_t length)
{
  for (uint16_t i = 0; i < length; i++)
  {
    writeByte(startAddress + i, data[i]);
  }
}

/**
 * Reads a block of data from SRAM starting from the specified address.
 *
 * @param startAddress The start address.
 * @param buffer Pointer to the buffer to store the data.
 * @param length Number of bytes to read.
 */
void HY62252A::readBlock(uint16_t startAddress, uint8_t *buffer, uint16_t length)
{
  for (uint16_t i = 0; i < length; i++)
  {
    buffer[i] = readByte(startAddress + i);
  }
}

/**
 * Stores a key-value pair in SRAM.
 * Key is 4 bytes, and value is 16 bytes.
 *
 * @param startAddress Address to store the key-value pair.
 * @param key 4-byte key to store.
 * @param value 16-byte value to store.
 */
void HY62252A::storeKeyValue(uint16_t startAddress, const char *key, const char *value)
{
  writeBlock(startAddress, (uint8_t *)key, 4);        // Store key
  writeBlock(startAddress + 4, (uint8_t *)value, 16); // Store value
}

/**
 * Searches for a key and retrieves the corresponding value.
 *
 * @param keyToFind The 4-byte key to search for.
 * @param valueBuffer Buffer to store the retrieved value (16 bytes).
 * @param startAddress Start address to search.
 * @param endAddress End address to search.
 * @return True if key found, false otherwise.
 */
bool HY62252A::getValueForKey(const char *keyToFind, char *valueBuffer, uint16_t startAddress, uint16_t endAddress)
{
  char keyBuffer[5] = {0};

  for (uint16_t addr = startAddress; addr < endAddress; addr += 20)
  {                                           // Each key-value pair occupies 20 bytes
    readBlock(addr, (uint8_t *)keyBuffer, 4); // Read key
    Logger::log(TRACE, "Key: " + String(keyBuffer));
    if (strncmp(keyBuffer, keyToFind, 4) == 0)
    {
      readBlock(addr + 4, (uint8_t *)valueBuffer, 16); // Read value
      return true;                                     // Key found
    }
  }
  return false; // Key not found
}