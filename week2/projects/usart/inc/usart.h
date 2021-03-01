#ifndef _USART_H_
#define _USART_H_

#include <stdlib.h> // For malloc() function
#include "stdint.h"

// Return value of dequeue function for when the buffer is empty
#define BUFFER_IS_EMPTY (255)

void USART_init(void);
void USART_putc(char c);
char USART_getc(void);
void USART_putstr(char *str);
void USART_getstr(char *str);
void USART_clearscreen(void);
char USART_dequeue(void);
void USART_enqueue(char input);

#endif // _USART_H_
