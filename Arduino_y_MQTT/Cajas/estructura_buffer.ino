#include "estructura_buffer.h"
portMUX_TYPE buffer_access = portMUX_INITIALIZER_UNLOCKED;
float put_item(float item)
{ 
  portENTER_CRITICAL(&buffer_access);
  
  if (buf.bufCount == BUFFER_SIZE)
   {
    portEXIT_CRITICAL(&buffer_access);
    return -1.0f;
  }
  buf.buffer[buf.bufIN] = item;
  buf.bufIN = (buf.bufIN + 1) % BUFFER_SIZE;
  buf.bufCount++;

  portEXIT_CRITICAL(&buffer_access);
  return 0.0f;
}

float get_item(float * item)
{
  portENTER_CRITICAL(&buffer_access);
  
  if (buf.bufCount == 0) 
  {
    portEXIT_CRITICAL(&buffer_access);
    return -1.0f;
  }
  *item = buf.buffer[buf.bufOUT];
  buf.bufOUT = (buf.bufOUT + 1) % BUFFER_SIZE;
  buf.bufCount--;

  portEXIT_CRITICAL(&buffer_access);
  return 0.0f;
}
