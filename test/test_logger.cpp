// test/test_logger.cpp
#include <Arduino.h>
#include <unity.h>
#include "test_utilities.h" // Include the mock serial utility
#include "logger.h"         // Include your existing logger

MockSerial mockSerial; // Use the mock serial object instead of the default HardwareSerial

void test_logger_log(void)
{
  mockSerial.clear(); // Clear any previous output
  Logger::log(INFO, "Hello, world!");

  // Check if the output matches the expected string (adjust as needed)
  String expectedOutput = "[0] INFO: Hello, world!\n"; // Example expected output
  TEST_ASSERT_EQUAL_STRING(expectedOutput.c_str(), mockSerial.output.c_str());
}

void setup()
{
  UNITY_BEGIN();
  RUN_TEST(test_logger_log);
  UNITY_END();
}

void loop()
{
  // Empty loop
}
