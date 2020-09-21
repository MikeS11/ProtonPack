/**********************************************************************
 *
 * This is a quick library for the Holtek HT16K33 LED driver + keyscan
 * IC. It’s not functionally exhaustive by any stretch, but it should
 * at least work reasonably well.
 *
 * @TODO: build functionality to read key data from the chip.
 *
 **********************************************************************/
#include <Wire.h>
#include "HT16K33.h"

/**
 * Utility function to flip a 16-bit integer. There may be better ways of doing this—let me know!
 */
uint16_t _flip_uint16(uint16_t in)
{
  uint16_t out = 0;
  
  for (uint8_t i = 0; i < 16; i++)
  {
    out <<= 1;
    out |= in & 1;
    in >>= 1;
  }

  return out;
}

// Constructor
void HT16K33::init(uint8_t addr)
{
  // orientation flags
  resetOrientation();
  
  // set the I2C address
  _i2c_addr = addr;
  
  // assign + zero some buffer data
  _buffer = (uint16_t*)calloc(8, sizeof(uint16_t));
  
  // start everything
  Wire.begin();
  Wire.beginTransmission(_i2c_addr);
  Wire.write(0x21); // turn it on
  Wire.endTransmission();
  
  // set blink off + brightness all the way up
  setBlink(HT16K33_BLINK_OFF);
  setBrightness(15);
  
  // write the matrix, just in case
  write();
}

/**
 * Sets the brightness of the display.
 */
void HT16K33::setBrightness(uint8_t brightness)
{
  // constrain the brightness to a 4-bit number (0–15)
  brightness = brightness & 0x0F;
  
  // send the command
  Wire.beginTransmission(_i2c_addr);
  Wire.write(HT16K33_CMD_DIMMING | brightness);
  Wire.endTransmission();
}

/**
 * Set the blink rate.
 */
void HT16K33::setBlink(uint8_t blink)
{
  Wire.beginTransmission(_i2c_addr);
  Wire.write(HT16K33_CMD_SETUP | HT16K33_DISPLAY_ON | blink);
  Wire.endTransmission();
}

/**
 * Reset the matrix orientation./
 */
void HT16K33::resetOrientation(void)
{
  _reversed = false;
  _vFlipped = false;
  _hFlipped = false;
}

/**
 * Flips the order of the two 8x8 matrices: useful if you’ve wired them backward by mistake =)
 */
void HT16K33::reverse(void)
{
  _reversed = !_reversed;
}

/**
 * Flips the vertical orientation of the matrices.
 */
void HT16K33::flipVertical(void)
{
  _vFlipped = !_vFlipped;
}

/**
 * Flips the vertical orientation of the matrices.
 */
void HT16K33::flipHorizontal(void)
{
  _hFlipped = !_hFlipped;;
}


/**
 * Clears the display buffer. Note that this doesn’t clear the display—you’ll need to call write() to do this.
 */
void HT16K33::clear(void)
{  
  for (uint8_t i = 0; i < 8; i++)
  {
    _buffer[i] = 0;
  }  
}

/**
 * Sets the value of a particular pixel.
 */
void HT16K33::setPixel(uint8_t col, uint8_t row, uint8_t val)
{
  // bounds checking
  col = col & 0x0F;
  row = row & 0x07;
  val = val & 0x01; 
  
  // write the buffer
  if (val == 1)
  {
    _buffer[row] |= 1 << col;
  }
  else
  {
    _buffer[row] &= ~(1 << col);
  }

}

/**
 * Sets the value of an entire row.
 */
void HT16K33::setRow(uint8_t row, uint16_t value)
{
  // bound check the row
  row = row & 0x07;
  
  // write it
  _buffer[row] = value;
}

/**
 * Set the value of an entire column. This is more fun =)
 */
void HT16K33::setColumn(uint8_t col, uint8_t value)
{
  // just do this via set pixel—waaaay easier!
  for (uint8_t row = 0; row < 8; row++)
  {
    setPixel(col, row, (value & (1 << row)) > 0);
  }
}

/**
 * Bulk-writes a set of row data to the display.
 */
void HT16K33::drawSprite16(Sprite16 sprite, uint8_t colOffset, uint8_t rowOffset)
{
  // iterate through data and set stuff
  for (uint8_t row = 0; row < sprite.height(); row++)
  {
    _buffer[(row + rowOffset) & 0x07] |= (sprite.readRow(row) << colOffset) & 0xFFFF;
  }
  
}

/**
 * Same as the above, just without offsets.
 */
void HT16K33::drawSprite16(Sprite16 sprite)
{
  drawSprite16(sprite, 0, 0);
} 

/**
 * Write the RAM buffer to the matrix.
 */
void HT16K33::write(void)
{
  Wire.beginTransmission(_i2c_addr);
  Wire.write(HT16K33_CMD_RAM);
  
  for (uint8_t row = 0; row < 8; row++)
  {
    writeRow(row);
  }
  
  Wire.endTransmission();
}

/**
 * Write a row to the chip.
 */
void HT16K33::writeRow(uint8_t row)
{
  // flip vertically
  if (_vFlipped)
  {
    row = 7 - row;
  }
  
  // read out the buffer so we can flip horizontally
  uint16_t out = _buffer[row];
  if (_hFlipped)
  {
    out = _flip_uint16(out);
  }
  
  if (_reversed)
  {
    Wire.write(out >> 8); // second byte
    Wire.write(out & 0xFF); // first byte
  }
  else
  {
    Wire.write(out & 0xFF); // first byte
    Wire.write(out >> 8); // second byte
  }
}