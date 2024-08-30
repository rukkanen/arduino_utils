#include "SRAM_I2C.h"
#ifdef ESP8266
#include <algorithm>
#else
#include <Arduino.h>
#endif

/**
 * @brief Constructor for the SRAM_I2C class.
 *
 * Initializes the SRAM_I2C class with a given I2C device address.
 * The device address is stored in a private variable for use in all
 * subsequent operations. The Wire.begin() function initializes the I2C
 * communication.
 *
 * @param deviceAddress The I2C address of the SRAM module.
 */
SRAM_I2C::SRAM_I2C(uint8_t deviceAddress)
    : _deviceAddress(deviceAddress)
{
  // Initialize the I2C communication
  Wire.begin();
}

/**
 * @brief Writes a single byte to a specific memory address in the SRAM.
 *
 * This function sends a byte of data to a specified address within the SRAM.
 * The memory address is sent first, followed by the data byte.
 * The function checks the success of the I2C transmission and returns
 * true if successful.
 *
 * @param memoryAddress The address in the SRAM where the data will be written.
 * @param data The byte of data to be written to the specified address.
 * @return true if the write operation was successful, false otherwise.
 */
bool SRAM_I2C::writeByte(uint16_t memoryAddress, uint8_t data)
{
  // Begin I2C transmission to the SRAM module
  Wire.beginTransmission(_deviceAddress);
  // Send the memory address (16-bit split into two 8-bit bytes)
  _sendMemoryAddress(memoryAddress);
  // Send the data byte to be written
  Wire.write(data);
  // End the transmission and check if it was successful
  return (Wire.endTransmission() == 0);
}

/**
 * @brief Reads a single byte from a specific memory address in the SRAM.
 *
 * This function reads a byte of data from a specified address within the SRAM.
 * It first sends the memory address, then requests the data byte from the SRAM.
 * The function returns the byte read from the SRAM.
 *
 * @param memoryAddress The address in the SRAM from which the data will be read.
 * @return The byte of data read from the specified address in the SRAM.
 */
uint8_t SRAM_I2C::readByte(uint16_t memoryAddress)
{
  // Begin I2C transmission to the SRAM module
  Wire.beginTransmission(_deviceAddress);
  // Send the memory address (16-bit split into two 8-bit bytes)
  _sendMemoryAddress(memoryAddress);
  // End the transmission
  Wire.endTransmission();
  // Request 1 byte of data from the SRAM module
  Wire.requestFrom(_deviceAddress, (uint8_t)1);
  // Return the byte read from the SRAM, or 0 if no data was received
  return (Wire.available()) ? Wire.read() : 0;
}

/**
 * @brief Writes multiple bytes to the SRAM starting from a specific address.
 *
 * This function writes a sequence of bytes to the SRAM, starting from a
 * specified address. It handles the I2C communication in chunks that fit
 * within the I2C transmission limit. The function returns true if all bytes
 * were successfully written.
 *
 * @param memoryAddress The starting address in the SRAM where the data will be written.
 * @param data A pointer to the array of data to be written to the SRAM.
 * @param length The number of bytes to write from the data array to the SRAM.
 * @return true if the write operation was successful, false otherwise.
 */
bool SRAM_I2C::writeBytes(uint16_t memoryAddress, const uint8_t *data, size_t length)
{
  // Continue writing until all bytes are written
  while (length > 0)
  {
    // Begin I2C transmission to the SRAM module
    Wire.beginTransmission(_deviceAddress);
    // Send the memory address (16-bit split into two 8-bit bytes)
    _sendMemoryAddress(memoryAddress);

    // Determine the number of bytes to write in this chunk
    size_t bytesToWrite = std::min(length, (size_t)30); // 30 bytes max to stay within I2C limits
    // Write the data bytes
    Wire.write(data, bytesToWrite);
    // End the transmission and check if it was successful
    if (Wire.endTransmission() != 0)
    {
      return false; // Return false if there was an error
    }

    // Update pointers and counters
    memoryAddress += bytesToWrite; // Move to the next memory address block
    data += bytesToWrite;          // Move the data pointer
    length -= bytesToWrite;        // Decrease the number of bytes left to write
  }

  return true; // Return true if the write operation was successful
}

/**
 * @brief Reads multiple bytes from the SRAM starting from a specific address.
 *
 * This function reads a sequence of bytes from the SRAM, starting from a
 * specified address, and stores them in a provided buffer. It handles the I2C
 * communication in chunks that fit within the I2C transmission limit. The
 * function returns true if all bytes were successfully read.
 *
 * @param memoryAddress The starting address in the SRAM from which the data will be read.
 * @param buffer A pointer to the buffer where the read data will be stored.
 * @param length The number of bytes to read from the SRAM into the buffer.
 * @return true if the read operation was successful, false otherwise.
 */
bool SRAM_I2C::readBytes(uint16_t memoryAddress, uint8_t *buffer, size_t length)
{
  // Continue reading until all bytes are read
  while (length > 0)
  {
    // Begin I2C transmission to the SRAM module
    Wire.beginTransmission(_deviceAddress);
    // Send the memory address (16-bit split into two 8-bit bytes)
    _sendMemoryAddress(memoryAddress);
    // End the transmission and check if it was successful
    if (Wire.endTransmission() != 0)
    {
      return false; // Return false if there was an error
    }

    // Determine the number of bytes to read in this chunk
    size_t bytesToRead = std::min(length, (size_t)32); // 32 bytes max to stay within I2C limits
    // Request the data bytes from the SRAM module
    Wire.requestFrom(_deviceAddress, (uint8_t)bytesToRead);
    // Read the data into the buffer
    for (size_t i = 0; i < bytesToRead && Wire.available(); ++i)
    {
      buffer[i] = Wire.read();
    }

    // Update pointers and counters
    memoryAddress += bytesToRead; // Move to the next memory address block
    buffer += bytesToRead;        // Move the buffer pointer
    length -= bytesToRead;        // Decrease the number of bytes left to read
  }

  return true; // Return true if the read operation was successful
}

/**
 * @brief Sends the memory address over I2C.
 *
 * This private function splits the 16-bit memory address into two 8-bit
 * bytes and sends them over I2C. The MSB (most significant byte) is sent first,
 * followed by the LSB (least significant byte). This function is used
 * internally by the public read and write methods to specify the memory
 * location for the subsequent data transfer.
 *
 * @param memoryAddress The 16-bit memory address to be sent to the SRAM module.
 */
void SRAM_I2C::_sendMemoryAddress(uint16_t memoryAddress)
{
  // Send the most significant byte (MSB) of the memory address
  Wire.write((memoryAddress >> 8) & 0xFF);
  // Send the least significant byte (LSB) of the memory address
  Wire.write(memoryAddress & 0xFF);
}
