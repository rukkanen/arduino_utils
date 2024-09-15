#include <Arduino.h>
#include "HY62252A.h"
#include "ShiftRegister74HC595.h"
#include "logger.h"
#if LOG_LEVEL == 3
// Define whether you're using shift registers or direct GPIO for the address lines
#define USE_SHIFT_REGISTERS true

#if SIMPLE_SHIFTER_TEST
uint8_t latchPin = 4;
uint8_t clockPin = 5;
uint8_t dataPin = 6;

// Create the shift register object with 1 register
ShiftRegister74HC595 shifter1(latchPin, clockPin, dataPin, 1);

// Define the test pins connected to Q0-Q7 of the shift register
uint8_t outputPins[8] = {7, 8, 9, 10, 11, 12, 13, A0}; // Test pins for Q0-Q7

// Struct to store results of each test
struct TestResult
{
  String testName;
  bool passed;
  String failingPins;
};

// Array to store the results of all tests
TestResult testResults[4];

// Forward declarations of test functions
void minimalTimingTest();
void randomizedDelayTest();
void boundaryAddressTest();
void backToBackWriteReadTest();
void finalReport();
void runTest(const char *testName, void (*testFunc)());

// Helper function to run a test and collect its results
void runTest(const char *testName, void (*testFunc)())
{
  static int testIndex = 0;
  Serial.print("\nRunning ");
  Serial.println(testName);
  testResults[testIndex].testName = testName;
  testResults[testIndex].passed = true;
  testResults[testIndex].failingPins = "";

  // Run the test function
  testFunc();

  // Store the result
  if (testResults[testIndex].passed)
  {
    Serial.println("Test PASSED.");
  }
  else
  {
    Serial.println("Test FAILED.");
  }
  testIndex++;
}

// Test 1: Minimal Timing (No Delays Between Operations)
void minimalTimingTest()
{
  bool localTestPassed = true;
  String localFailingPins = "";

  // Test each pin by setting HIGH and LOW with no delay
  for (uint8_t i = 0; i < 8; i++)
  {
    // Set pin HIGH with no delay
    shifter1.setPin(i, HIGH);
    int pinState = digitalRead(outputPins[i]);
    if (pinState != HIGH)
    {
      localTestPassed = false;
      localFailingPins += "Pin " + String(i) + " HIGH failed; ";
    }

    // Immediately set pin LOW with no delay
    shifter1.setPin(i, LOW);
    pinState = digitalRead(outputPins[i]);
    if (pinState != LOW)
    {
      localTestPassed = false;
      localFailingPins += "Pin " + String(i) + " LOW failed; ";
    }
  }

  testResults[0].passed = localTestPassed;
  testResults[0].failingPins = localFailingPins;
}

// Test 2: Randomized Delays Between Operations
void randomizedDelayTest()
{
  bool localTestPassed = true;
  String localFailingPins = "";

  // Randomized delay between setting HIGH and LOW
  for (uint8_t i = 0; i < 8; i++)
  {
    // Set pin HIGH with a random delay
    shifter1.setPin(i, HIGH);
    delay(random(1, 50)); // Random delay between 1 and 50 ms
    int pinState = digitalRead(outputPins[i]);
    if (pinState != HIGH)
    {
      localTestPassed = false;
      localFailingPins += "Pin " + String(i) + " HIGH failed; ";
    }

    // Set pin LOW with a random delay
    shifter1.setPin(i, LOW);
    delay(random(1, 50)); // Random delay between 1 and 50 ms
    pinState = digitalRead(outputPins[i]);
    if (pinState != LOW)
    {
      localTestPassed = false;
      localFailingPins += "Pin " + String(i) + " LOW failed; ";
    }
  }

  testResults[1].passed = localTestPassed;
  testResults[1].failingPins = localFailingPins;
}

// Test 3: Boundary Address Test (Focus on Address Transitions)
void boundaryAddressTest()
{
  bool localTestPassed = true;
  String localFailingPins = "";

  // Test boundary conditions such as 0x00FF and 0x0100
  uint16_t boundaryAddresses[] = {0x00FF, 0x0100}; // Critical boundary addresses
  uint8_t boundaryPatterns[] = {0xAA, 0x55};       // Test patterns

  for (uint8_t a = 0; a < sizeof(boundaryAddresses) / sizeof(boundaryAddresses[0]); a++)
  {
    // Write test pattern
    uint8_t pattern = boundaryPatterns[a % 2];
    for (uint8_t i = 0; i < 8; i++)
    {
      shifter1.setPin(i, (pattern >> i) & 1);
    }
    delay(10); // Give time for shift register update

    // Read back and validate
    for (uint8_t i = 0; i < 8; i++)
    {
      int pinState = digitalRead(outputPins[i]);
      if (pinState != ((pattern >> i) & 1))
      {
        localTestPassed = false;
        localFailingPins += "Pin " + String(i) + " boundary address failed; ";
      }
    }
  }

  testResults[2].passed = localTestPassed;
  testResults[2].failingPins = localFailingPins;
}

// Test 4: Back-to-Back Write and Read (No Significant Delay)
void backToBackWriteReadTest()
{
  bool localTestPassed = true;
  String localFailingPins = "";

  // Stress test by writing and reading back immediately with no significant delay
  for (uint8_t i = 0; i < 8; i++)
  {
    // Write HIGH, read immediately
    shifter1.setPin(i, HIGH);
    int pinState = digitalRead(outputPins[i]);
    if (pinState != HIGH)
    {
      localTestPassed = false;
      localFailingPins += "Pin " + String(i) + " HIGH failed; ";
    }

    // Write LOW, read immediately
    shifter1.setPin(i, LOW);
    pinState = digitalRead(outputPins[i]);
    if (pinState != LOW)
    {
      localTestPassed = false;
      localFailingPins += "Pin " + String(i) + " LOW failed; ";
    }
  }

  testResults[3].passed = localTestPassed;
  testResults[3].failingPins = localFailingPins;
}

// Final report: Summarizes test results and lists passing/failing tests
void finalReport()
{
  Serial.println("\n--- FINAL TEST REPORT ---");

  for (int i = 0; i < 4; i++)
  {
    Serial.print(testResults[i].testName + ": ");
    if (testResults[i].passed)
    {
      Serial.println("PASSED.");
    }
    else
    {
      Serial.println("FAILED.");
      Serial.print("Failing pins: ");
      Serial.println(testResults[i].failingPins);
    }
  }
}

#endif // SIMPLE_SHIFTER_TEST

#if TEST_WITH_SRAM
#if USE_SHIFT_REGISTERS
// Shift register pin definitions (adjust to your wiring)
uint8_t latchPin = 4;
uint8_t clockPin = 5;
uint8_t dataPin = 6;

// Define two shift registers (for lower and upper address bits)
ShiftRegister74HC595 shiftRegister1(latchPin, clockPin, dataPin, 1); // Handles lower address bits
ShiftRegister74HC595 shiftRegister2(latchPin, clockPin, dataPin, 1); // Handles upper address bits

// Data and control pin definitions
uint8_t dataPins[] = {A0, A1, A2, A3, A4, A5, A6, A7}; // I/O pins (D0-D7)
uint8_t cePin = 8;                                     // Chip Enable (/CE)
uint8_t oePin = 9;                                     // Output Enable (/OE)
uint8_t wePin = 10;                                    // Write Enable (/WE)

// Create SRAM object using two shift registers
HY62252A sram(&shiftRegister1, &shiftRegister2, dataPins, cePin, oePin, wePin, 8, 7); // 8 bits in first, 7 in second

#else
// Define the address pins (if not using shift registers)
uint8_t addressPins[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, A0, A1, A2}; // A0-A14 (adjust to your GPIO pinout)

// Data and control pin definitions
uint8_t dataPins[] = {A3, A4, A5, A6, A7, 14, 15, 16}; // I/O pins (D0-D7)
uint8_t cePin = 8;                                     // Chip Enable (/CE)
uint8_t oePin = 9;                                     // Output Enable (/OE)
uint8_t wePin = 10;                                    // Write Enable (/WE)

// Create SRAM object using direct GPIO
HY62252A sram(addressPins, dataPins, cePin, oePin, wePin);

#endif

void fullTestShifterSRAM()
{

  Logger::info("SRAM Test - Full Range Check");

  // Initialize SRAM
  sram.begin();
  Logger::info("SRAM initialized.");
  delay(3000);

  // Loop to test multiple addresses across the SRAM
  uint8_t testDataWrite = 0xAA; // Sample data to write (0xAA)
  uint8_t testDataRead;
  bool testPassed = true;

  uint8_t testData[5] = {0xAA, 0x55, 0xFF, 0x00, 0x77};                 // Example test data
  uint16_t testAddresses[5] = {0x0000, 0x0100, 0x0200, 0x0400, 0x0800}; // Test addresses

  Logger::info("-----------------------------Starting thorough writeByte and readByte test...");

  for (int i = 0; i < 5; i++)
  {
    Logger::info("------ TESTING ADDRESS: 0x" + String(testAddresses[i], HEX));

    // Write the test data to the SRAM
    sram.writeByte(testAddresses[i], testData[i]);

    // Read the data back from the same address
    uint8_t readData = sram.readByte(testAddresses[i]);

    // Check if the read data matches the written data
    if (readData == testData[i])
    {
      Logger::info("PASSED: Address 0x" + String(testAddresses[i], HEX) + " Data matches: " + String(testData[i], HEX));
    }
    else
    {
      Logger::info("FAILED: Address 0x" + String(testAddresses[i], HEX) + " Data mismatch: Written: " +
                   String(testData[i], HEX) + " Read: " + String(readData, HEX));
    }
  }

  Logger::info("-----------------------------Starting address range tests...");

  int passes = 0;
  int fails = 0;

  // Test a range of addresses across SRAM (e.g., step by 0x100 to spread out)
  for (uint16_t address = 0x0000; address <= 0x0300; address += 0x0100)
  {
    Logger::info("------ TESTING ADDRESS: 0x" + String(address, HEX));

    // Write to the address
    sram.writeByte(address, testDataWrite);

    // Read back from the address
    testDataRead = sram.readByte(address);

    // Check if the read data matches the written data
    if (testDataRead == testDataWrite)
    {
      passes++;
      Logger::info("PASSED --------- : Address 0x" + String(address, HEX) + " PASSED. Data matches.");
    }
    else
    {
      fails++;
      Logger::info("FAILED --------- : Address 0x" + String(address, HEX) + " FAILED. Data mismatch.");
      testPassed = false; // Mark the test as failed
    }
  }

  // Final result
  if (testPassed)
  {
    Logger::info("All address range tests PASSED.");
  }
  else
  {
    Logger::info("Some address range tests FAILED. Check wiring or address handling.");
    Logger::info("PASSED: " + String(passes) + ", FAILED: " + String(fails));
  }

  // Optionally, add a delay between tests to slow things down for observation
  delay(1000);

  Logger::info("SRAM test completed.");
}
#endif
void setup()
{
#if SIMPLE_SHIFTER_TEST
  // Start the serial monitor
  Serial.begin(115200);
  delay(1000);

  // Initialize the test pins as inputs
  for (uint8_t i = 0; i < 8; i++)
  {
    pinMode(outputPins[i], INPUT);
  }

  Serial.println("Shift Register Timing Stress Test: Arduino Uno");

  // Run all tests
  runTest("Minimal Timing Test", minimalTimingTest);
  runTest("Randomized Delay Test", randomizedDelayTest);
  runTest("Boundary Address Test", boundaryAddressTest);
  runTest("Back-to-Back Write/Read Test", backToBackWriteReadTest);

  // Final report
  finalReport();
#else
  fullTestShifterSRAM();
#endif
}

void loop()
{
  // Nothing to do in the loop
}
#endif
