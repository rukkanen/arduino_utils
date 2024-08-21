#ifndef LOGGER_H
#define LOGGER_H

#include <Arduino.h>

/**
 * Log levels
 * INFO: Informational messages
 * WARNING: Warning messages
 * ERROR: Error messages
 *
 * To configure the log level, change the value of the `logLevel` variable
 */
enum LogLLevel
{
  INFO,
  WARNING,
  ERROR,
  CRITICAL
};
void logLMessage(LogLLevel level, const String &message);

#endif
