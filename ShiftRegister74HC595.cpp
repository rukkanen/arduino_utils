#include "ShiftRegister74HC595.h"

// Constructor
ShiftRegister74HC595::ShiftRegister74HC595(uint8_t latchPin, uint8_t clockPin, uint8_t dataPin, uint8_t numRegisters)
    : _latchPin(latchPin), _clockPin(clockPin), _dataPin(dataPin), _numRegisters(numRegisters)
{

  // Initialize the latch, clock, and data pins
  pinMode(_latchPin, OUTPUT);
  pinMode(_clockPin, OUTPUT);
  pinMode(_dataPin, OUTPUT);

  // Allocate memory for the register states
  _registerState = new uint8_t[_numRegisters];

  // Initialize all register states to 0 (LOW)
  initRegisters();
}

// Private method to initialize the shift registers
void ShiftRegister74HC595::initRegisters()
{
  for (uint8_t i = 0; i < _numRegisters; i++)
  {
    _registerState[i] = 0; // Set all to LOW initially
  }
  updateRegisters(); // Ensure the hardware is in sync with the initial state
}

// Set the state of a specific pin
void ShiftRegister74HC595::setPin(uint8_t pin, bool state)
{
  uint8_t registerIndex = pin / 8; // Determine which register this pin is on
  uint8_t bitIndex = pin % 8;      // Determine which bit in the register corresponds to the pin

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
  for (uint8_t i = 0; i < _numRegisters; i++)
  {
    _registerState[i] = 0;
  }
  updateRegisters();
}

// Set all pins (set all to HIGH)
void ShiftRegister74HC595::setAll()
{
  for (uint8_t i = 0; i < _numRegisters; i++)
  {
    _registerState[i] = 0xFF;
  }
  updateRegisters();
}

// Update the shift registers (push the changes to the actual hardware)
void ShiftRegister74HC595::updateRegisters()
{
  digitalWrite(_latchPin, LOW); // Begin the update by setting the latch low

  // Send out the bytes for each shift register, starting with the last one
  for (int i = _numRegisters - 1; i >= 0; i--)
  {
    shiftOut(_dataPin, _clockPin, MSBFIRST, _registerState[i]);
  }

  digitalWrite(_latchPin, HIGH); // Complete the update by setting the latch high
}
