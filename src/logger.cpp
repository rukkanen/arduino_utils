#include "logger.h"

/**
 * Log a message with a timestamp and log level
 * @param level The log level
 * @param message The message to log
 * @return void
 *
 * To configure the log level, change the value of the `logLevel` variable
 */
void logLMessage(LogLLevel level, const String &message)
{
  const char *levelStr;
  switch (level)
  {
  case INFO:
    levelStr = "INFO";
    break;
  case WARNING:
    levelStr = "WARNING";
    break;
  case ERROR:
    levelStr = "ERROR";
    break;
  case CRITICAL:
    levelStr = "CRITICAL";
    break;
  }
  Serial.print("[");
  Serial.print(millis());
  Serial.print("] ");
  Serial.print(levelStr);
  Serial.print(": ");
  Serial.println(message);
}
