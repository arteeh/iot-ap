#ifndef USART_H
#define USART_H

#include "stm32f0xx.h"

void initUsart(void);
void uPutChar(char c);
void uPutString(char *str);
void uClearScreen(void);
char * uIntToAscii(int16_t i, char *p);
void delay(const int d);

#endif // USART_H
