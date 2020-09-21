/**
 * This is a quick demo of the orientation settings for this library, which allow you to flip matrices in software
 * without having to do much in the way of hard work.
 */
#include "HT16K33.h"

HT16K33 matrix = HT16K33();

void setup() 
{
  // start the matrix on I2C address 0x70
  matrix.init(0x70);  
  
  // draw an arrow on the display
  matrix.setRow(1, 30);
  matrix.setRow(2,  2);
  matrix.setRow(3, 10);
  matrix.setRow(4, 18);
  matrix.setRow(5, 32);
  
  // hold off
  delay(100);
}

void loop() 
{
  // show it the right way around
  matrix.write();
  delay(2000);
  
  // flip horizontally
  matrix.flipHorizontal();
  matrix.write();
  delay(2000);
  matrix.resetOrientation();
  
  // flip vertically 
  matrix.flipVertical();
  matrix.write();
  delay(2000);
  matrix.resetOrientation();
  
  // flipping both ways: this is the same as rotating through 180°
  matrix.flipHorizontal();
  matrix.flipVertical();
  matrix.write();
  delay(2000);
  matrix.resetOrientation();
}
