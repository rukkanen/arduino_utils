#include "logger.h"

// Logger function that checks log level before printing
void Logger::log(LogLevel level, const String &message)
{
  if (level <= LOG_LEVEL) // Only log if level is <= compile-time LOG_LEVEL
  {
    const char *levelStr;
    switch (level)
    {
    case LogLevel::ERROR:
      levelStr = "ERROR";
      break;
    case LogLevel::WARNING:
      levelStr = "WARNING";
      break;
    case LogLevel::INFO:
      levelStr = "INFO";
      break;
    case LogLevel::TRACE:
      levelStr = "TRACE";
      break;
    default:
      levelStr = "UNKNOWN";
    }
    Serial.print("[");
    Serial.print(millis());
    Serial.print("] ");
    Serial.print(levelStr);
    Serial.print(": ");
    Serial.println(message);
  }
}
