#include "ShiftRegister74HC595.h"
#include "logger.h"

// Constructor
ShiftRegister74HC595::ShiftRegister74HC595(uint8_t latchPin, uint8_t clockPin, uint8_t dataPin, uint8_t numRegisters)
    : _latchPin(latchPin), _clockPin(clockPin), _dataPin(dataPin), _numRegisters(numRegisters)
{
  Logger::info("Initializing ShiftRegister74HC595 with " + String(numRegisters) + " registers.");

  // Initialize the latch, clock, and data pins
  pinMode(_latchPin, OUTPUT);
  pinMode(_clockPin, OUTPUT);
  pinMode(_dataPin, OUTPUT);

  // Allocate memory for the register states
  _registerState = new uint8_t[_numRegisters];

  // Initialize all register states to 0 (LOW)
  initRegisters();
}

ShiftRegister74HC595::~ShiftRegister74HC595()
{
  delete[] _registerState;
}

// Private method to initialize the shift registers
void ShiftRegister74HC595::initRegisters()
{
  Logger::info("Initializing registers...");

  for (uint8_t i = 0; i < _numRegisters; i++)
  {
    _registerState[i] = 0; // Set all to LOW initially
    Logger::trace("Register " + String(i) + " initialized to LOW.");
  }
  updateRegisters(); // Ensure the hardware is in sync with the initial state
}

// Set the state of a specific pin
void ShiftRegister74HC595::setPin(uint8_t pin, bool state)
{
  uint8_t registerIndex = pin / 8; // Determine which register this pin is on
  uint8_t bitIndex = pin % 8;      // Determine which bit in the register corresponds to the pin

  // Log pin and register info
  Logger::trace("Setting pin " + String(pin) + " on register " + String(registerIndex) + " to " + String(state));

  // Set or clear the bit in the register's state array
  if (state)
  {
    _registerState[registerIndex] |= (1 << bitIndex);
  }
  else
  {
    _registerState[registerIndex] &= ~(1 << bitIndex);
  }

  updateRegisters(); // Update the hardware with the new state
}

// Clear all pins (set all to LOW)
void ShiftRegister74HC595::clearAll()
{
  Logger::info("Clearing all registers.");
  for (uint8_t i = 0; i < _numRegisters; i++)
  {
    _registerState[i] = 0;
    Logger::trace("Register " + String(i) + " cleared.");
  }
  updateRegisters();
}

// Set all pins (set all to HIGH)
void ShiftRegister74HC595::setAll()
{
  Logger::info("Setting all registers to HIGH.");
  for (uint8_t i = 0; i < _numRegisters; i++)
  {
    _registerState[i] = 0xFF;
    Logger::trace("Register " + String(i) + " set to HIGH.");
  }
  updateRegisters();
}

// Update the shift registers (push the changes to the actual hardware)
void ShiftRegister74HC595::updateRegisters()
{
  Logger::info("Updating shift registers...");

  digitalWrite(_latchPin, LOW); // Begin the update by setting the latch low
  Logger::trace("Latch pin " + String(_latchPin) + " set to LOW.");

  // Send out the bytes for each shift register, starting with the last one
  for (int i = _numRegisters - 1; i >= 0; i--)
  {
    uint8_t shiftedByte = _registerState[i];
    Logger::trace("Shifting out byte: 0x" + String(shiftedByte, HEX) + " for register " + String(i));
    shiftOut(_dataPin, _clockPin, MSBFIRST, shiftedByte);
    Logger::trace("Data shifted out to register " + String(i));
  }

  digitalWrite(_latchPin, HIGH); // Complete the update by setting the latch high
  Logger::trace("Latch pin " + String(_latchPin) + " set to HIGH.");

  // Small delay to ensure registers have time to settle
  delayMicroseconds(5);
  Logger::trace("Registers updated and latched.");
}
