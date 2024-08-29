#include "LCD1602IIC.h"

/**
 * Constructor
 *
 * Initializes the LCD1602IIC object with the specified I2C address,
 * number of columns, and number of rows. The constructor also initializes
 * the underlying LiquidCrystal_I2C object.
 *
 * @param lcdAddr The I2C address of the LCD screen (default is 0x27).
 * @param lcdCols The number of columns on the LCD screen (default is 16).
 * @param lcdRows The number of rows on the LCD screen (default is 2).
 */
LCD1602IIC::LCD1602IIC(uint8_t lcdAddr, uint8_t lcdCols, uint8_t lcdRows)
    : lcd(lcdAddr, lcdCols, lcdRows), cols(lcdCols), rows(lcdRows) {}

/**
 * begin
 *
 * Initializes the LCD by calling the begin() method of the LiquidCrystal_I2C
 * object. This method also turns on the backlight by default.
 */
void LCD1602IIC::begin()
{
  lcd.begin();     // Initialize the LCD screen
  lcd.backlight(); // Turn on the backlight
}

/**
 * clear
 *
 * Clears all characters from the display and resets the cursor position
 * to the top-left corner (0, 0).
 */
void LCD1602IIC::clear()
{
  lcd.clear(); // Clear the display
}

/**
 * home
 *
 * Sets the cursor position to the home position (0, 0).
 */
void LCD1602IIC::home()
{
  lcd.home(); // Set cursor to (0, 0)
}

/**
 * setCursor
 *
 * Sets the cursor position to the specified column and row.
 * Ensures the column and row are within the LCD's limits before
 * attempting to set the cursor position.
 *
 * @param col The column number (0-based index).
 * @param row The row number (0-based index).
 */
void LCD1602IIC::setCursor(uint8_t col, uint8_t row)
{
  if (col < cols && row < rows)
  {
    lcd.setCursor(col, row); // Set cursor to the specified position
  }
}

/**
 * backlight
 *
 * Turns on the backlight of the LCD screen.
 */
void LCD1602IIC::backlight()
{
  lcd.backlight(); // Enable backlight
}

/**
 * noBacklight
 *
 * Turns off the backlight of the LCD screen.
 */
void LCD1602IIC::noBacklight()
{
  lcd.noBacklight(); // Disable backlight
}

/**
 * print
 *
 * Prints a C-style string (null-terminated character array) to the LCD.
 *
 * @param message The text to print on the LCD screen.
 */
void LCD1602IIC::print(const char *message)
{
  lcd.print(message); // Print C-string to LCD
}

/**
 * print
 *
 * Prints a String object to the LCD.
 *
 * @param message The String object to print on the LCD screen.
 */
void LCD1602IIC::print(const String &message)
{
  lcd.print(message); // Print String object to LCD
}

/**
 * print
 *
 * Prints an integer to the LCD.
 *
 * @param number The integer to print on the LCD screen.
 */
void LCD1602IIC::print(int number)
{
  lcd.print(number); // Print integer to LCD
}

/**
 * print
 *
 * Prints a floating-point number to the LCD with a specified number of decimal places.
 *
 * @param number The floating-point number to print on the LCD screen.
 * @param decimals The number of decimal places to display (default is 2).
 */
void LCD1602IIC::print(float number, int decimals)
{
  lcd.print(number, decimals); // Print float to LCD with specified decimal places
}

/**
 * cursor
 *
 * Displays an underline cursor at the current cursor position.
 */
void LCD1602IIC::cursor()
{
  lcd.cursor(); // Show underline cursor
}

/**
 * noCursor
 *
 * Hides the underline cursor, making it invisible.
 */
void LCD1602IIC::noCursor()
{
  lcd.noCursor(); // Hide underline cursor
}

/**
 * blink
 *
 * Makes the LCD cursor blink on and off at the current cursor position.
 */
void LCD1602IIC::blink()
{
  lcd.blink(); // Enable blinking cursor
}

/**
 * noBlink
 *
 * Disables the blinking of the cursor, making it static.
 */
void LCD1602IIC::noBlink()
{
  lcd.noBlink(); // Disable blinking cursor
}
