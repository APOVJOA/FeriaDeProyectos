#ifndef ESTRUCTURA_BUFFER_H
#define ESTRUCTURA_BUFFER_H


#define BUFFER_SIZE 10

struct Buffer_Circ 
{
  float buffer[BUFFER_SIZE];
  uint16_t bufIN;
  uint16_t bufOUT;
  uint32_t bufCount;
};
Buffer_Circ buf;

#endif