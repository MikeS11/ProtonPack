/**
 * Quick and dirty 16-bit sprite library.
 */

#ifndef Sprite16_h

  // include appropriate version of Arduino code
  #if (ARDUINO >= 100)
    #include "Arduino.h"
  #else
    #include "WProgram.h"
  #endif

  #define Sprite16_h
  
  class Sprite16
  {
    public:
      Sprite16(uint8_t width, uint8_t height, uint16_t data, ...);
      uint8_t width();
      uint8_t height();
      uint16_t readRow(uint8_t row);
  
    private:
      uint8_t   _height;
      uint8_t   _width;
      uint16_t *_buffer;
      
      void init(uint8_t width, uint8_t height);
  };
#endif