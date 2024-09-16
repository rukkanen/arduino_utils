#ifndef SHIFTREGISTER74HC595_H
#define SHIFTREGISTER74HC595_H

#include <Arduino.h>
#include <SPI.h>

class ShiftRegister74HC595
{
public:
  // Constructor
  ShiftRegister74HC595(uint8_t latchPin, uint8_t clockPin, uint8_t dataPin, uint8_t numRegisters = 1);
  ~ShiftRegister74HC595();

  // Set the state of a specific pin
  void setPin(uint8_t pin, bool state);

  // Clear all pins (set all to LOW)
  void clearAll();

  // Set all pins (set all to HIGH)
  void setAll();

  // Update the shift registers (push the changes to the actual hardware)
  void updateRegisters();

private:
  uint8_t _latchPin;
  uint8_t _clockPin;
  uint8_t _dataPin;
  uint8_t _numRegisters;
  uint8_t *_registerState; // Array to hold the state of each register

  // Internal function to initialize the shift registers
  void initRegisters();
};

#endif
