#ifndef MAIN_H
#define MAIN_H

#include "stm32f0xx.h"
#include "stm32f0xx_spi.h"
#include "stm32f0_discovery.h"
#include "graphics.h"

#define SECOND (SystemCoreClock/8)

void initGpio(void);
void initSpi(void);
void delay(const int d);

#endif
