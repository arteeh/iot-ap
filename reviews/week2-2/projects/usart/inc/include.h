#ifndef INCLUDE_H_
#define INCLUDE_H_
#define BUFFER_SIZE 10

#include <stdbool.h>
#include <stdlib.h>
#include "usart.h"
#include "stm32f0xx.h"

bool isBufferFull(void);
bool isBufferEmpty(void);
void addToBuffer(char data);
char readFromBuffer(void);

#endif
