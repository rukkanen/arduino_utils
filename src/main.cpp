#include <Arduino.h>
#include "HY62252A.h"

#ifdef UNO

// Pin Definitions (as per the pin matrix provided)
uint8_t addr_pins[] = {0, 1, 2, 3, 4, 5, 6, 7, 15, 14, 11, 13, A6, A7, 17}; // Address pins A0 to A14
uint8_t data_pins[] = {A0, A1, A2, A3, A4, A5, 8, 9};                       // Data pins I/O1 to I/O8

// Control pins
uint8_t ce_pin = 10; // Chip Enable
uint8_t oe_pin = 12; // Output Enable
uint8_t we_pin = 16; // Write Enable

// Create an instance of the HY62252A class (direct GPIO control)
HY62252A sram(addr_pins, data_pins, ce_pin, oe_pin, we_pin);

void testSingleByte()
{
  Serial.println("\nTest 1: Single Byte Write/Read Test");

  // Sample addresses and data to test
  uint16_t testAddresses[] = {0x0000, 0x0100, 0x0200, 0x0400, 0x0800}; // Addresses to test
  uint8_t testData[] = {0xAA, 0x55, 0xFF, 0x00, 0x77};                 // Data to write

  for (int i = 0; i < 5; i++)
  {
    uint16_t address = testAddresses[i];
    uint8_t dataToWrite = testData[i];

    // Write data to the SRAM
    sram.writeByte(address, dataToWrite);
    Serial.print("Written 0x");
    Serial.print(dataToWrite, HEX);
    Serial.print(" to address 0x");
    Serial.println(address, HEX);

    // Read data back from SRAM
    uint8_t dataRead = sram.readByte(address);
    Serial.print("Read 0x");
    Serial.print(dataRead, HEX);
    Serial.print(" from address 0x");
    Serial.println(address, HEX);

    // Verify data matches
    if (dataRead == dataToWrite)
    {
      Serial.println("Test PASSED");
    }
    else
    {
      Serial.println("Test FAILED");
    }
  }
}

void testBlockWriteRead()
{
  Serial.println("\nTest 2: Block Write/Read Test");

  uint16_t startAddress = 0x0100;                             // Starting address for block write
  uint8_t writeData[] = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF}; // Data block to write
  uint8_t readBuffer[6];                                      // Buffer to store the read data

  // Write a block of data to SRAM
  sram.writeBlock(startAddress, writeData, 6);
  Serial.print("Written block to address 0x");
  Serial.println(startAddress, HEX);

  // Read the block back from SRAM
  sram.readBlock(startAddress, readBuffer, 6);
  Serial.print("Read block from address 0x");
  Serial.println(startAddress, HEX);

  // Verify the data matches
  bool testPassed = true;
  for (int i = 0; i < 6; i++)
  {
    Serial.print("Read 0x");
    Serial.print(readBuffer[i], HEX);
    Serial.print(" Expected 0x");
    Serial.println(writeData[i], HEX);

    if (readBuffer[i] != writeData[i])
    {
      Serial.println("Data mismatch!");
      testPassed = false;
    }
  }

  if (testPassed)
  {
    Serial.println("Block Write/Read Test PASSED");
  }
  else
  {
    Serial.println("Block Write/Read Test FAILED");
  }
}

void testKeyValueStorage()
{
  Serial.println("\nTest 3: Key-Value Storage Test");

  uint16_t kvStartAddress = 0x0200;          // Starting address for key-value storage
  const char testKey[] = "KEY1";             // 4-byte key
  const char testValue[] = "HelloSRAM12345"; // 16-byte value

  // Store the key-value pair
  sram.storeKeyValue(kvStartAddress, testKey, testValue);
  Serial.println("Stored key-value pair.");

  // Retrieve the value for the key
  char retrievedValue[17]; // Buffer to store the retrieved value
  bool keyFound = sram.getValueForKey(testKey, retrievedValue, kvStartAddress, 0x0300);

  // Verify the key-value retrieval
  if (keyFound)
  {
    Serial.print("Key found. Retrieved value: ");
    Serial.println(retrievedValue);

    // Check if the retrieved value matches the original
    if (strcmp(retrievedValue, testValue) == 0)
    {
      Serial.println("Key-Value Storage Test PASSED");
    }
    else
    {
      Serial.println("Key-Value Storage Test FAILED: Value mismatch");
    }
  }
  else
  {
    Serial.println("Key-Value Storage Test FAILED: Key not found");
  }
}

void setup()
{
  // Initialize serial for debugging
  Serial.begin(115200);
  delay(2000); // Allow time for the system to stabilize
  Serial.println("SRAM Test - Direct GPIO Control");

  // Initialize the SRAM module
  sram.begin();
  Serial.println("SRAM initialized.");

  // Test 1: Single-byte read/write test
  testSingleByte();

  // Test 2: Block read/write test
  testBlockWriteRead();

  // Test 3: Key-Value storage test
  testKeyValueStorage();
}

void loop()
{
  // The main loop remains empty as we only want to perform tests in setup
}
#else 
void setup() {
  // put your setup code here, to run once:

}
void loop() {
  // put your main code here, to run repeatedly:

}