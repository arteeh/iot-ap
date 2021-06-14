#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <stdint.h>
#include "stm32f0xx_spi.h"
#include "main.h"

// Intensity of the leds, should be between 0 and 15
#define matrixIntensity		0

// All the registers of max7192 that we can write to
#define matrixRegNoop					0x00
#define matrixRegDecodeMode		0x09
#define matrixRegIntensity		0x0a
#define matrixRegScanLimit		0x0b
#define matrixRegShutdown			0x0c
#define matrixRegDisplayTest	0x0f

extern uint8_t buffer[8];

void matrixInit(void);
void matrixClear(void);
void matrixUpdate(void);
void matrixWrite(uint8_t address, uint8_t data);

void flash(void);
void setLetter(uint8_t ascii);
void setBall(void);
void shiftLeft(void);
void shiftRight(void);
void shiftUp(void);
void shiftDown(void);


#endif
