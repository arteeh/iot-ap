#ifndef DISPLAY_H
#define DISPLAY_H

#include "stm32f0xx_spi.h"

// Intensity of the leds, should be between 0 and 15
#define displayIntensity			0

// All the registers of max7192 that we can write to
#define displayRegNoop				0x00
#define displayRegDecodeMode	0x09
#define displayRegIntensity		0x0a
#define displayRegScanLimit		0x0b
#define displayRegShutdown		0x0c
#define displayRegDisplayTest	0x0f

void display(void);
void displayClear(void);
void displayUpdate(void);
void displayWrite(uint8_t address, uint8_t data);

#endif // DISPLAY_H
