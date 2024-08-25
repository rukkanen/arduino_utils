#include <gtest/gtest.h>
#include "logger.h"

// Mock Serial for testing
class MockSerial
{
public:
  void begin(int baudrate) {}
  void print(const char *msg) { lastMessage += msg; }
  void println(const char *msg)
  {
    lastMessage += msg;
    lastMessage += "\n";
  }
  String lastMessage;
};

MockSerial Serial;

TEST(LoggerTest, LogsInfoMessages)
{
  // Setup
  Serial.lastMessage = "";
  logMessage(INFO, "Test Info Message");

  // Test
  EXPECT_TRUE(Serial.lastMessage.find("INFO") != String::npos);
  EXPECT_TRUE(Serial.lastMessage.find("Test Info Message") != String::npos);
}

TEST(LoggerTest, LogsWarningMessages)
{
  // Setup
  Serial.lastMessage = "";
  logMessage(WARNING, "Test Warning Message");

  // Test
  EXPECT_TRUE(Serial.lastMessage.find("WARNING") != String::npos);
  EXPECT_TRUE(Serial.lastMessage.find("Test Warning Message") != String::npos);
}

TEST(LoggerTest, LogsErrorMessages)
{
  // Setup
  Serial.lastMessage = "";
  logMessage(ERROR, "Test Error Message");

  // Test
  EXPECT_TRUE(Serial.lastMessage.find("ERROR") != String::npos);
  EXPECT_TRUE(Serial.lastMessage.find("Test Error Message") != String::npos);
}

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
