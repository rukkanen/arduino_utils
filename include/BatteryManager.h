#ifndef BATTERY_MANAGER_H
#define BATTERY_MANAGER_H

#include "logger.h"

/**
 * Battery manager class to handle battery level monitoring
 * and low battery mode
 *
 * Make a separate intance of this class for each battery pack.
 *
 * My plan is to:
 * - add a possibility for an interrupt pin to wake up the manager
 * - to
 *
 * The batteries in question are 18650 Li-ion batteries with 3,7V nominal voltage:
 * 4.2V: 100% (Fully charged)
 * 4.0V: ~85% charge
 * 3.7V: ~50% charge
 * 3.5V: ~25% charge
 * 3.2V: ~0-5% charge (Considered nearly empty)
 * <3.0V: Risk of damaging the battery if further discharged
 */

struct BatteryThresholds
{
  int warningThreshold;
  int criticalThreshold;
  int shutdownThreshold;
};

class BatteryManager
{
public:
  /**
   * Constructor for the BatteryManager class
   * The batteries in question are 18650 Li-ion batteries with 3,7V nominal voltage
   * But maybe you can use this for other liion batteries as well
   *
   * @param batteryPin The pin where the battery voltage is read from
   * @param batteryThreshold The % battery level where the robot should enter deep sleep mode
   * @param amountOfBatteries The amount of batteries in the battery pack
   * @param theoreticalMaxVoltage The theoretical max voltage of the battery pack
   * @return A new BatteryManager object
   */
  BatteryManager(int batteryPin, int batteryThreshold, int amountOfBatteries, float theoreticalMaxVoltage);
  BatteryManager(int batteryPin);
  float getBatteryAdjustedLevel();
  bool isBatteryCritical();
  void setBatteryThresholds(int warningThreshold, int criticalThreshold, int shutdownThreshold);

private:
  BatteryThresholds thresholds = {25, 10, 5};
  int batteryPin;
  int batteryThreshold;
  int amountOfBatteries = 2;
  float theoreticalMaxVoltage = 4.2;
  float getBatteryPercentage();
  float currentVoltage;
};
;

#endif // BATTERY_MANAGER_H