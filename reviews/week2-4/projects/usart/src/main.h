#ifndef MAIN_H
#define MAIN_H

#include "stdint.h"
#include "stdlib.h"

#define DELAY_100MS SystemCoreClock/8/10

char *RXBuffer;
uint8_t bufferHead, bufferTail, bufferBegin, bufferEnd, bufferLength, offset;

void init(void);
void updateSerial(void);
void delay(const int d);

#endif
