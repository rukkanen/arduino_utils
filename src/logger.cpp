#include "logger.h"

void Logger::log(LogLevel level, const String &message)
{
  const char *levelStr;
  switch (level)
  {
  case LogLevel::INFO:
    levelStr = "INFO";
    break;
  case LogLevel::WARNING:
    levelStr = "WARNING";
    break;
  case LogLevel::ERROR:
    levelStr = "ERROR";
    break;
  case LogLevel::CRITICAL:
    levelStr = "CRITICAL";
    break;
  default:
    levelStr = "N/A";
  }
  Serial.print("[");
  Serial.print(millis());
  Serial.print("] ");
  Serial.print(levelStr);
  Serial.print(": ");
  Serial.println(message);
}
