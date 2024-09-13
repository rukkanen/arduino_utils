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
    case LogLevel::ULTRA:
      levelStr = "ULTRA";
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

void Logger::warning(const String &message)
{
  log(LogLevel::WARNING, message);
}

void Logger::error(const String &message)
{
  log(LogLevel::ERROR, message);
}

void Logger::info(const String &message)
{
  log(LogLevel::INFO, message);
}

void Logger::trace(const String &message)
{
  log(LogLevel::TRACE, message);
}

void Logger::setLogLevel(LogLevel newLevel)
{
  currentLogLevel = newLevel;
}

void Logger::ultra(const String &message)
{
  log(LogLevel::ULTRA, message);
}
