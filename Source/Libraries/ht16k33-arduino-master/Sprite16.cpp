#include "Sprite16.h"

Sprite16::Sprite16(uint8_t width, uint8_t height, uint16_t data, ...)
{
  // create everything
  init(width, height);
  
  // read in the data
  va_list ap;
  va_start(ap, data);
  for (uint8_t row = 0; ; row++)
  {
    _buffer[row] = data;
    
    if (row == _height - 1)
    {
      break;
    }
    
    data = va_arg(ap, uint16_t);
  }
  va_end(ap);
}

void Sprite16::init(uint8_t width, uint8_t height)
{
  // store height and width
  _width  = min(width, 16);
  _height = min(height, 8);
  
  // create the buffer
  _buffer = (uint16_t *)calloc(_height, sizeof(uint16_t));
}

uint8_t Sprite16::height(void)
{
  return _height;
}

uint8_t Sprite16::width(void)
{
  return _width;
}

uint16_t Sprite16::readRow(uint8_t row)
{
  if (row > _height)
  {
    return 0;
  }
  
  return _buffer[row];
}