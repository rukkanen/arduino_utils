// Need to comment this out due to arduino, for now.
// I'm sorry...
#ifdef ESP8266

#include "EEPROM24LC32A.h"
#ifdef ESP8266
#include <algorithm>
// NodeMCU-specific code
#elif defined(ARDUINO)
#include <Arduino.h>
#endif
/**
 * @brief Constructor for the EEPROM24LC32A class.
 *
 * Initializes the I2C device with a given address. The 24LC32A EEPROM
 * typically uses the address 0x50, but this can be adjusted based on the
 * hardware configuration (A0, A1, A2 pins).
 *
 * @param deviceAddress The I2C address of the EEPROM. Default is 0x50.
 */
EEPROM24LC32A::EEPROM24LC32A(uint8_t deviceAddress)
    : _deviceAddress(deviceAddress)
{
  // Initialize the I2C communication
  Wire.begin();
}

/**
 * @brief Writes a single byte to a specific memory address.
 *
 * This function writes a byte of data to a specified address in the EEPROM.
 * Since the EEPROM has limited write cycles, avoid excessive writing to
 * prolong its lifespan.
 *
 * @param memoryAddress The address in the EEPROM where the data will be written.
 * @param data The byte of data to be written.
 * @return true if the write was successful, false otherwise.
 */
bool EEPROM24LC32A::writeByte(uint16_t memoryAddress, uint8_t data)
{
  // Begin I2C transmission to the EEPROM device
  Wire.beginTransmission(_deviceAddress);
  // Send the most significant byte (MSB) of the memory address
  Wire.write((memoryAddress >> 8) & 0xFF);
  // Send the least significant byte (LSB) of the memory address
  Wire.write(memoryAddress & 0xFF);
  // Send the data byte to be written to the EEPROM
  Wire.write(data);
  // End the transmission and check if it was successful
  if (Wire.endTransmission() != 0)
  {
    return false; // Return false if there was an error
  }

  // Wait for the EEPROM to complete the write operation
  _waitForWrite();
  return true; // Return true if the write was successful
}

/**
 * @brief Reads a single byte from a specific memory address.
 *
 * This function reads a byte of data from a specified address in the EEPROM.
 *
 * @param memoryAddress The address in the EEPROM from which the data will be read.
 * @return The byte of data read from the EEPROM.
 */
uint8_t EEPROM24LC32A::readByte(uint16_t memoryAddress)
{
  // Begin I2C transmission to the EEPROM device
  Wire.beginTransmission(_deviceAddress);
  // Send the most significant byte (MSB) of the memory address
  Wire.write((memoryAddress >> 8) & 0xFF);
  // Send the least significant byte (LSB) of the memory address
  Wire.write(memoryAddress & 0xFF);
  // End the transmission and check if it was successful
  if (Wire.endTransmission() != 0)
  {
    return 0; // Return 0 if there was an error
  }

  // Request 1 byte of data from the EEPROM
  Wire.requestFrom(_deviceAddress, (uint8_t)1);
  // If data is available, read and return it
  if (Wire.available())
  {
    return Wire.read();
  }

  return 0; // Return 0 if no data was available
}

/**
 * @brief Writes multiple bytes to the EEPROM starting from a specific address.
 *
 * This function writes a sequence of bytes to the EEPROM. Writing multiple
 * bytes at once can be more efficient than writing individual bytes.
 *
 * @param memoryAddress The starting address in the EEPROM where the data will be written.
 * @param data A pointer to the array of data to be written.
 * @param length The number of bytes to write.
 * @return true if the write was successful, false otherwise.
 */
bool EEPROM24LC32A::writeBytes(uint16_t memoryAddress, const uint8_t *data, size_t length)
{
  // Write data in chunks that fit within the I2C transmission limit
  while (length > 0)
  {
    // Begin I2C transmission to the EEPROM device
    Wire.beginTransmission(_deviceAddress);
    // Send the most significant byte (MSB) of the memory address
    Wire.write((memoryAddress >> 8) & 0xFF);
    // Send the least significant byte (LSB) of the memory address
    Wire.write(memoryAddress & 0xFF);

    // Determine the number of bytes to write in this chunk (maximum 30 bytes)
    size_t bytesToWrite = std::min(length, (size_t)30);
    // Write the chunk of data
    Wire.write(data, bytesToWrite);
    // End the transmission and check if it was successful
    if (Wire.endTransmission() != 0)
    {
      return false; // Return false if there was an error
    }

    // Wait for the EEPROM to complete the write operation
    _waitForWrite();

    // Update the memory address, data pointer, and remaining length
    memoryAddress += bytesToWrite;
    data += bytesToWrite;
    length -= bytesToWrite;
  }

  return true; // Return true if the write was successful
}

/**
 * @brief Reads multiple bytes from the EEPROM starting from a specific address.
 *
 * This function reads a sequence of bytes from the EEPROM into a provided buffer.
 *
 * @param memoryAddress The starting address in the EEPROM from which the data will be read.
 * @param buffer A pointer to the buffer where the read data will be stored.
 * @param length The number of bytes to read.
 * @return true if the read was successful, false otherwise.
 */
bool EEPROM24LC32A::readBytes(uint16_t memoryAddress, uint8_t *buffer, size_t length)
{
  // Read data in chunks that fit within the I2C transmission limit
  while (length > 0)
  {
    // Begin I2C transmission to the EEPROM device
    Wire.beginTransmission(_deviceAddress);
    // Send the most significant byte (MSB) of the memory address
    Wire.write((memoryAddress >> 8) & 0xFF);
    // Send the least significant byte (LSB) of the memory address
    Wire.write(memoryAddress & 0xFF);
    // End the transmission and check if it was successful
    if (Wire.endTransmission() != 0)
    {
      return false; // Return false if there was an error
    }

    // Determine the number of bytes to read in this chunk (maximum 32 bytes)
    size_t bytesToRead = std::min(length, (size_t)32);
    // Request the chunk of data from the EEPROM
    Wire.requestFrom(_deviceAddress, (uint8_t)bytesToRead);
    // Read the data into the buffer
    for (size_t i = 0; i < bytesToRead && Wire.available(); ++i)
    {
      buffer[i] = Wire.read();
    }

    // Update the memory address, buffer pointer, and remaining length
    memoryAddress += bytesToRead;
    buffer += bytesToRead;
    length -= bytesToRead;
  }

  return true; // Return true if the read was successful
}

/**
 * @brief Waits until the EEPROM is ready for the next operation.
 *
 * The 24LC32A EEPROM takes some time to complete a write operation.
 * This function ensures that the EEPROM is ready before another operation is performed.
 * It does this by polling the EEPROM until it acknowledges, indicating it is ready.
 */
void EEPROM24LC32A::_waitForWrite()
{
  // Continuously try to initiate communication with the EEPROM until it responds
  while (true)
  {
    Wire.beginTransmission(_deviceAddress);
    // If the EEPROM acknowledges, it is ready
    if (Wire.endTransmission() == 0)
    {
      break; // Exit the loop when the EEPROM is ready
    }
  }
}
#endif // The header