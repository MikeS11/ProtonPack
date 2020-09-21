/**
 * This is a quick and dirty demo/test to demonstrate how to set rows,
 * columns, and individual pixels, as well as drawing sprites on a 16x8
 * matrix.
 *
 * Note that this will work fine with an 8x8 matrix or even a bicolour
 * matrix… although the latter might look a little strange.
 */
#include "HT16K33.h"

HT16K33 matrix = HT16K33();
Sprite16 smile = Sprite16( 6, 6, 18, 18, 0, 0, 33, 30);
Sprite16 frown = Sprite16( 6, 6, 18, 18, 0, 0, 30, 33);

void setup() 
{
  // start the matrix on I2C address 0x70
  matrix.init(0x70);  
  delay(1000);
}

void loop() 
{
  // draw some sprites
  matrix.drawSprite16(smile, 1, 1);
  matrix.drawSprite16(frown, 9, 1);
  matrix.write();
  
  // fade in
  int brt;
  for (brt = 0; brt < 16; brt++)
  {
    matrix.setBrightness(brt);
    delay(10);
  }
  delay(1000);
  for (brt = 15; brt >= 0; brt--)
  {
    matrix.setBrightness(brt);
    delay(10);
  }
  matrix.clear();
  matrix.setBrightness(15);
  
  // draw some spirals
  spiral8(0, 1);
  spiral8(8, 1);
  spiral16(0);
  delay(200);

  // wipe
  uint8_t row;
  matrix.clear();
  for (row = 0; row < 8; row++)
  {
    matrix.setRow(row, 0xFFFF);
    matrix.write();
    delay(50);
  }
  for (row = 0; row < 8; row++)
  {
    matrix.setRow(row, 0);
    matrix.write();
    delay(50);
  }

  // another wipe
  uint8_t col;
  for (col = 0; col < 16; col++)
  {
    matrix.setColumn(col, 0xFF);
    matrix.write();
    delay(50);
  }
  for (col = 0; col < 16; col++)
  {
    matrix.setColumn(col, 0);
    matrix.write();
    delay(50);
  }
  delay(1000);
  matrix.clear();
}

void spiral8(uint8_t start, uint8_t value)
{
  //start looping
  int row;
  int col;
  for (int fiddle = 0; fiddle < 4; fiddle++)
  {
    for (row = (start + fiddle); row < (start + (7 - fiddle)); row++) { pixel(row, fiddle, value); }
    for (col = fiddle; col < (7 - fiddle); col++) { pixel(start + (7 - fiddle), col, value); }
    for (row = (start + (7 - fiddle)); row > (start + fiddle); row--) { pixel(row, (7-fiddle), value); }
    for (col = (7 - fiddle); col > fiddle; col--) { pixel(start + fiddle, col, value); }
  }
}

void spiral16(uint8_t value)
{
  //start looping
  int row;
  int col;
  for (int fiddle = 0; fiddle < 4; fiddle++)
  {
    for (row = fiddle; row < (15 - fiddle); row++) { pixel(row, fiddle, value); }
    for (col = fiddle; col < (7 - fiddle); col++)  { pixel((15 - fiddle), col, value); }
    for (row = (15 - fiddle); row > fiddle; row--) { pixel(row, (7 - fiddle), value); }
    for (col = (7 - fiddle); col > fiddle; col--)  { pixel(fiddle, col, value); }
  }
}

void pixel( uint8_t row, uint8_t col, uint8_t val )
{
  matrix.setPixel(row, col, val);
  matrix.write();
  delay(10);
}