#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "stm32f0xx.h"

// Intensity of the leds, should be between 0 and 15
#define displayIntensity		0

// The registers of max7192 we can write to
#define displayRegNoop			0x00
#define displayRegDecodeMode	0x09
#define displayRegIntensity		0x0a
#define displayRegScanLimit		0x0b
#define displayRegShutdown		0x0c
#define displayRegDisplayTest	0x0f

// Buffer is used by game.c as well
extern uint8_t buffer[9];

void displayInit(void);
void displayClear(void);
void displayUpdate(void);
void displayWrite(uint8_t address, uint8_t data);

#endif
