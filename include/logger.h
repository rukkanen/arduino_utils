#ifndef LOGGER_H
#define LOGGER_H

#include <Arduino.h>

/**
 * Log levels
 * INFO: Informational messages
 * WARNING: Warning messages
 * ERROR: Error messages
 * TRACE: Absolutely everything
 * ULTRA: Absolutely everything and even more
 *
 * To configure the log level, change the value of the `logLevel` variable
 */
enum LogLevel
{
  ERROR = 0,
  WARNING = 1,
  INFO = 2,
  TRACE = 3,
  ULTRA = 4
};

// Check if LOG_LEVEL is defined, if not, set a default value
#ifndef LOG_LEVEL
#define LOG_LEVEL INFO
#endif

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
  static void warning(const String &message);
  static void error(const String &message);
  static void info(const String &message);
  static void trace(const String &message);
  static void ultra(const String &message);

  static void setLogLevel(LogLevel newLevel);

private:
  static LogLevel currentLogLevel;
};

#endif
