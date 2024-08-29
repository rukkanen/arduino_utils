#ifndef LCD1602IIC_H
#define LCD1602IIC_H

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

/**
 * LCD1602IIC Class
 *
 * This class provides an interface to control a 1602 I2C LCD screen
 * using the NodeMCUv2 (ESP8266). It wraps around the LiquidCrystal_I2C
 * library to offer easier methods for common tasks such as printing
 * text, controlling the cursor, and managing the display backlight.
 *
 * Class Description: Explains the purpose of the LCD1602IIC class,
 * which is to simplify interaction with a 1602 I2C LCD.
 *
 * Constructor: Details the constructor's parameters and their default
 * values, which are typical for most 1602 I2C LCD modules.
 *
 * Method Descriptions: Each method is commented to describe its
 * functionality, parameters, and in some cases, the expected behavior.
 *
 * Private Members: Comments explain the role of private member variables,
 * such as storing the number of columns and rows of the LCD, and the
 * LiquidCrystal_I2C object for the actual communication with the display.
 *
 */
class LCD1602IIC
{
public:
  /**
   * Constructor
   *
   * Initializes an instance of the LCD1602IIC class.
   *
   * @param lcdAddr The I2C address of the LCD screen (default is 0x27).
   * @param lcdCols The number of columns on the LCD screen (default is 16).
   * @param lcdRows The number of rows on the LCD screen (default is 2).
   */
  LCD1602IIC(uint8_t lcdAddr = 0x27, uint8_t lcdCols = 16, uint8_t lcdRows = 2);

  /**
   * begin
   *
   * Initializes the LCD screen and turns on the backlight.
   */
  void begin();

  /**
   * clear
   *
   * Clears all characters from the display and sets the cursor to (0, 0).
   */
  void clear();

  /**
   * home
   *
   * Sets the cursor position to the home position (0, 0).
   */
  void home();

  /**
   * setCursor
   *
   * Sets the cursor position to the specified column and row.
   *
   * @param col The column number (0-based index).
   * @param row The row number (0-based index).
   */
  void setCursor(uint8_t col, uint8_t row);

  /**
   * backlight
   *
   * Turns on the backlight of the LCD screen.
   */
  void backlight();

  /**
   * noBacklight
   *
   * Turns off the backlight of the LCD screen.
   */
  void noBacklight();

  /**
   * print
   *
   * Prints a C-style string (null-terminated character array) to the LCD.
   *
   * @param message The text to print on the LCD screen.
   */
  void print(const char *message);

  /**
   * print
   *
   * Prints a String object to the LCD.
   *
   * @param message The String object to print on the LCD screen.
   */
  void print(const String &message);

  /**
   * print
   *
   * Prints an integer to the LCD.
   *
   * @param number The integer to print on the LCD screen.
   */
  void print(int number);

  /**
   * print
   *
   * Prints a floating-point number to the LCD with a specified number of decimal places.
   *
   * @param number The floating-point number to print on the LCD screen.
   * @param decimals The number of decimal places to display (default is 2).
   */
  void print(float number, int decimals = 2);

  /**
   * cursor
   *
   * Displays an underline cursor at the current cursor position.
   */
  void cursor();

  /**
   * noCursor
   *
   * Hides the underline cursor.
   */
  void noCursor();

  /**
   * blink
   *
   * Makes the LCD cursor blink on and off at the current cursor position.
   */
  void blink();

  /**
   * noBlink
   *
   * Disables the blinking of the cursor.
   */
  void noBlink();

private:
  LiquidCrystal_I2C lcd; // Object to interface with the I2C LCD
  uint8_t cols;          // Number of columns on the LCD
  uint8_t rows;          // Number of rows on the LCD
};

#endif // LCD1602IIC_H
