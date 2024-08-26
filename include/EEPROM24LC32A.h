#ifndef EEPROM24LC32A_H
#define EEPROM24LC32A_H

#include <Wire.h> // Include the Wire library for I2C communication

/**
 * @class EEPROM24LC32A
 * @brief A class to interface with the 24LC32A EEPROM over I2C.
 *
 * This class provides methods to read from and write to the 24LC32A EEPROM,
 * which is a 32Kb (4KB) non-volatile memory chip. The EEPROM is accessed over
 * I2C, and this class abstracts the low-level details of that communication.
 */
class EEPROM24LC32A
{
public:
  /**
   * @brief Constructor for the EEPROM24LC32A class.
   *
   * Initializes the I2C device with a given address. The 24LC32A EEPROM
   * typically uses the address 0x50, but this can be adjusted based on the
   * hardware configuration (A0, A1, A2 pins).
   *
   * @param deviceAddress The I2C address of the EEPROM. Default is 0x50.
   */
  EEPROM24LC32A(uint8_t deviceAddress = 0x50);

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
  bool writeByte(uint16_t memoryAddress, uint8_t data);

  /**
   * @brief Reads a single byte from a specific memory address.
   *
   * This function reads a byte of data from a specified address in the EEPROM.
   *
   * @param memoryAddress The address in the EEPROM from which the data will be read.
   * @return The byte of data read from the EEPROM.
   */
  uint8_t readByte(uint16_t memoryAddress);

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
  bool writeBytes(uint16_t memoryAddress, const uint8_t *data, size_t length);

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
  bool readBytes(uint16_t memoryAddress, uint8_t *buffer, size_t length);

private:
  uint8_t _deviceAddress; // The I2C device address of the EEPROM

  /**
   * @brief Waits until the EEPROM is ready for the next operation.
   *
   * The 24LC32A EEPROM takes some time to complete a write operation.
   * This function ensures that the EEPROM is ready before another operation is performed.
   * It does this by polling the EEPROM until it acknowledges, indicating it is ready.
   */
  void _waitForWrite();
};

#endif // EEPROM24LC32A_H
