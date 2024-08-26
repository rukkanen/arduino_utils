// test_utilities.h
#ifndef TEST_UTILITIES_H
#define TEST_UTILITIES_H

#include <Arduino.h>

class MockSerial
{
public:
  String output;

  void begin(int baud) {}
  void print(const char *str)
  {
    output += str;
  }
  void println(const char *str)
  {
    output += str;
    output += "\n";
  }
  void clear()
  {
    output = "";
  }
};

#endif // TEST_UTILITIES_H
