#ifdef UTILS_MAIN
#include <Arduino.h>
#include "logger.h"
#include "BatteryManager.h"
#include "EEPROM24LC32A.h"
#include "ShiftRegister74HC595.h"

BatteryManager batteryManager(0);
ShiftRegister74HC595 shiftRegister(2, 3, 4);
void setup()
{
  batteryManager = BatteryManager(0);
  // eeprom = EEPROM24LC32A();
  shiftRegister = ShiftRegister74HC595(2, 3, 4);

  // log = &Logger::gTuossaetInstance();
  Logger::log(INFO, "Hello, world!");
}

void loop()
{
  // TODO: Add your loop code here
}

#endif