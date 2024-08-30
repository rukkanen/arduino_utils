#ifndef SRAM_I2C_H
#define SRAM_I2C_H

#include <Wire.h> // Include the Wire library to facilitate I2C communication
#ifdef ESP8266
#include <algorithm>
// NodeMCU-specific code
#else
#include <EEPROM.h> // Use built-in EEPROM library for Arduino
#endif

/**
 * @class SRAM_I2C
 * @brief A class to interface with an I2C-based SRAM module.
 *
 * This class provides methods to read from and write to an SRAM module
 * over the I2C bus. SRAM (Static Random Access Memory) is a type of volatile
 * memory that allows fast read and write operations without the write cycle
 * limitations of EEPROM. However, unlike EEPROM, SRAM data is lost when power
 * is removed.
 */
class SRAM_I2C
{
public:
  /**
   * @brief Constructor for the SRAM_I2C class.
   *
   * Initializes the SRAM_I2C class with a given I2C device address.
   * This address is determined by the specific configuration of the SRAM
   * module's hardware (usually set via pins on the module).
   *
   * @param deviceAddress The I2C address of the SRAM module.
   */
  SRAM_I2C(uint8_t deviceAddress);

  /**
   * @brief Writes a single byte to a specific memory address in the SRAM.
   *
   * This function sends a byte of data to a specified address within the SRAM.
   * The SRAM module does not have the wear issues associated with EEPROM,
   * so it allows unlimited read and write cycles. This function handles the
   * necessary I2C communication to perform the write operation.
   *
   * @param memoryAddress The address in the SRAM where the data will be written.
   *                      The address is 16 bits, allowing access to a large
   *                      range of memory locations.
   * @param data The byte of data to be written to the specified address.
   * @return true if the write operation was successful, false otherwise.
   */
  bool writeByte(uint16_t memoryAddress, uint8_t data);

  /**
   * @brief Reads a single byte from a specific memory address in the SRAM.
   *
   * This function reads a byte of data from a specified address within the SRAM.
   * Since SRAM is volatile, the data will only persist while power is supplied
   * to the module. This function handles the necessary I2C communication to
   * retrieve the data.
   *
   * @param memoryAddress The address in the SRAM from which the data will be read.
   *                      The address is 16 bits, allowing access to a large
   *                      range of memory locations.
   * @return The byte of data read from the specified address in the SRAM.
   */
  uint8_t readByte(uint16_t memoryAddress);

  /**
   * @brief Writes multiple bytes to the SRAM starting from a specific address.
   *
   * This function writes a sequence of bytes to the SRAM, starting from a
   * specified address. Writing multiple bytes in a single operation can
   * be more efficient than writing them individually, especially for larger
   * data blocks. The function automatically handles the necessary I2C
   * communication to transfer the data.
   *
   * @param memoryAddress The starting address in the SRAM where the data will be written.
   *                      The address is 16 bits, allowing access to a large
   *                      range of memory locations.
   * @param data A pointer to the array of data to be written to the SRAM.
   * @param length The number of bytes to write from the data array to the SRAM.
   * @return true if the write operation was successful, false otherwise.
   */
  bool writeBytes(uint16_t memoryAddress, const uint8_t *data, size_t length);

  /**
   * @brief Reads multiple bytes from the SRAM starting from a specific address.
   *
   * This function reads a sequence of bytes from the SRAM, starting from a
   * specified address, and stores them in a provided buffer. Reading multiple
   * bytes in a single operation is more efficient than reading them individually,
   * particularly for larger data blocks. The function handles the necessary I2C
   * communication to retrieve the data.
   *
   * @param memoryAddress The starting address in the SRAM from which the data will be read.
   *                      The address is 16 bits, allowing access to a large
   *                      range of memory locations.
   * @param buffer A pointer to the buffer where the read data will be stored.
   * @param length The number of bytes to read from the SRAM into the buffer.
   * @return true if the read operation was successful, false otherwise.
   */
  bool readBytes(uint16_t memoryAddress, uint8_t *buffer, size_t length);

private:
  uint8_t _deviceAddress; // The I2C device address of the SRAM module

  /**
   * @brief Sends the memory address over I2C.
   *
   * This private function splits the 16-bit memory address into two 8-bit
   * bytes and sends them over I2C. This step is necessary because the I2C
   * protocol sends data one byte at a time. The function is used internally
   * by the public read and write methods to specify the memory location for
   * the subsequent data transfer.
   *
   * @param memoryAddress The 16-bit memory address to be sent to the SRAM module.
   */
  void _sendMemoryAddress(uint16_t memoryAddress);
};

#endif // SRAM_I2C_H
