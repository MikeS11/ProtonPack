/**
 * This is a quick demo of the reverse() functionality, which switches the order in which data bytes are sent to the
 * HT16K33, and is very useful if—like me—you wire your matrixes backward + really don’t want to have to redo everything.
 */
#include "HT16K33.h"
#include "Sprite16.h"

HT16K33 matrix = HT16K33();
Sprite16 one = Sprite16(3, 5, 2, 3, 2, 2, 7);
Sprite16 two = Sprite16(3, 5, 2, 5, 4, 2, 7);

void setup() 
{
  // start the matrix on I2C address 0x70
  matrix.init(0x70);
  
  // draw some quick identifiers
  matrix.drawSprite16(one,  3, 1);
  matrix.drawSprite16(two, 11, 1);
  
}

void loop() 
{
  // write everything the ‘correct’ way around
  matrix.write();
  delay(2000);

  // reverse the order of the matrixes, write again
  matrix.reverse();
  matrix.write();
  delay(2000);

  // reset ready to try again
  matrix.resetOrientation();  
}
