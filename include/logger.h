#ifndef LOGGER_H
#define LOGGER_H

#include <Arduino.h>

/**
 * Log levels
 * INFO: Informational messages
 * WARNING: Warning messages
 * ERROR: Error messages
 * CRITICAL: Not just an error, but something which should stop operation
 *
 * To configure the log level, change the value of the `logLevel` variable
 */
enum LogLevel
{
  INFO,
  WARNING,
  ERROR,
  CRITICAL
};

class Logger
{
public:
  /**
   * Log a message with a timestamp sand log level
   * @param level The log level
   * @param message The message to log
   * @return void
   *
   * To configure the log level, change the value of the `logLevel` variable
   */
  static void log(LogLevel level, const String &message);
};

#endif
