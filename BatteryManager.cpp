#include "BatteryManager.h"
#include <Arduino.h>
#include "logger.h"

/**
 * Constructor for the BatteryManager class
 * The batteries in question are 18650 Li-ion batteries with 3,7V nominal voltage
 */
BatteryManager::BatteryManager(int batteryPin, int batteryThreshold, int amountOfBatteries, float theoreticalMaxVoltage)
    : batteryPin(batteryPin), batteryThreshold(batteryThreshold), amountOfBatteries(amountOfBatteries), theoreticalMaxVoltage(theoreticalMaxVoltage)
{
  logLMessage(INFO, "Battery manager initialized");
}

BatteryManager::BatteryManager(int batteryPin)
    : batteryPin(batteryPin)
{
  logLMessage(INFO, "Battery manager initialized");
  theoreticalMaxVoltage = 4.2;
  amountOfBatteries = 2;
  batteryThreshold = 25;
}

float BatteryManager::getBatteryAdjustedLevel()
{
  currentVoltage = analogRead(batteryPin) * (theoreticalMaxVoltage / 1023.0);
  float batteryPercent = getBatteryPercentage();

  if (batteryPercent < batteryThreshold)
  {
    logLMessage(WARNING, "Battery level is " + String(batteryPercent) + " belov given threshold: " + String(batteryPercent) + "%");
  }
  return batteryPercent;
}

/**
 * Get the battery voltage
 * 4.2V: 100% (Fully charged)
 * 4.0V: ~85% charge
 * 3.7V: ~50% charge
 * 3.5V: ~25% charge
 * 3.2V: ~0-5% charge (Considered nearly empty)
 * <3.0V: Risk of damaging the battery if further discharged
 *
 * @return The "real" battery percentage
 */
float BatteryManager::getBatteryPercentage()
{
  int linearBatteryPercentage = 0;

  switch (static_cast<int>(currentVoltage * 10))
  {
  case 84 ... 100:
    linearBatteryPercentage = 100;
    break;
  case 80 ... 83:
    linearBatteryPercentage = 85;
    break;
  case 74 ... 79:
    linearBatteryPercentage = 50;
    break;
  case 70 ... 73:
    linearBatteryPercentage = 25;
    break;
  case 64 ... 69:
    linearBatteryPercentage = 10;
    break;
  }

  float a = 10.91;
  float b = -174.11;
  float c = 662.43;

  int algoritmicBatteryPercentage = static_cast<int>(a * currentVoltage * currentVoltage + b * currentVoltage + c);

  // Ensure the result is clamped between 0% and 100%
  if (algoritmicBatteryPercentage > 100)
    algoritmicBatteryPercentage = 100;
  if (algoritmicBatteryPercentage < 0)
    algoritmicBatteryPercentage = 0;

  logLMessage(INFO, "linearBatteryPercentage: " + String(linearBatteryPercentage));
  logLMessage(INFO, "algoritmicBatteryPercentage: " + String(algoritmicBatteryPercentage));
  // no surprise here, the linearBatteryPercentage is always going to be the one we can trust, maybe
  return linearBatteryPercentage;
}

bool BatteryManager::isBatteryCritical()
{
  float batteryPercent = getBatteryAdjustedLevel();
  if (batteryPercent < 10)
  {
    logLMessage(CRITICAL, "Battery level is critical: " + String(batteryPercent) + "%");
    return true;
  }
  return false;
}

void BatteryManager::setBatteryThresholds(int warningThreshold, int criticalThreshold, int shutdownThreshold)
{
  if (warningThreshold != -1)
    thresholds.warningThreshold = warningThreshold;
  if (criticalThreshold != -1)
    thresholds.criticalThreshold = criticalThreshold;
  if (shutdownThreshold != -1)
    thresholds.shutdownThreshold = shutdownThreshold;
}
