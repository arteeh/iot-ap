#include "graphics.h"

// The buffer to keep track of what's on the display without having to read the device itself
uint8_t buffer[8];

// This is a collection of 8 by 8 characters.
// It contains items 33 through 122 of the ASCII table
// Copied from: http://www.piclist.com/techref/datafile/charset/8x8.htm
const uint8_t characters[][8] = {
{
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},{// !
0x04,0x04,0x04,0x04,0x00,0x00,0x04,0x00},{// "
0x0A,0x0A,0x0A,0x00,0x00,0x00,0x00,0x00},{// #
0x0A,0x0A,0x1F,0x0A,0x1F,0x0A,0x0A,0x00},{// $
0x04,0x0F,0x14,0x0E,0x05,0x1E,0x04,0x00},{// %
0x18,0x19,0x02,0x04,0x08,0x13,0x03,0x00},{// &
0x0C,0x12,0x14,0x08,0x15,0x12,0x0D,0x00},{// '
0x0C,0x04,0x08,0x00,0x00,0x00,0x00,0x00},{// (
0x02,0x04,0x08,0x08,0x08,0x04,0x02,0x00},{// )
0x08,0x04,0x02,0x02,0x02,0x04,0x08,0x00},{// *
0x00,0x04,0x15,0x0E,0x15,0x04,0x00,0x00},{// +
0x00,0x04,0x04,0x1F,0x04,0x04,0x00,0x00},{// ,
0x00,0x00,0x00,0x00,0x0C,0x04,0x08,0x00},{// -
0x00,0x00,0x00,0x1F,0x00,0x00,0x00,0x00},{// .
0x00,0x00,0x00,0x00,0x00,0x0C,0x0C,0x00},{// /
0x00,0x01,0x02,0x04,0x08,0x10,0x00,0x00},{// 0
0x0E,0x11,0x13,0x15,0x19,0x11,0x0E,0x00},{// 1
0x04,0x0C,0x04,0x04,0x04,0x04,0x0E,0x00},{// 2
0x0E,0x11,0x01,0x02,0x04,0x08,0x1F,0x00},{// 3
0x1F,0x02,0x04,0x02,0x01,0x11,0x0E,0x00},{// 4
0x02,0x06,0x0A,0x12,0x1F,0x02,0x02,0x00},{// 5
0x1F,0x10,0x1E,0x01,0x01,0x11,0x0E,0x00},{// 6
0x06,0x08,0x10,0x1E,0x11,0x11,0x0E,0x00},{// 7
0x1F,0x01,0x02,0x04,0x04,0x04,0x04,0x00},{// 8
0x1E,0x11,0x11,0x0E,0x11,0x11,0x0E,0x00},{// 9
0x0E,0x11,0x11,0x0F,0x01,0x02,0x0C,0x00},{// :
0x00,0x0C,0x0C,0x00,0x0C,0x0C,0x00,0x00},{// ;
0x00,0x0C,0x0C,0x00,0x0C,0x04,0x08,0x00},{// <
0x02,0x04,0x08,0x10,0x08,0x04,0x02,0x00},{// =
0x00,0x00,0x1F,0x00,0x1F,0x00,0x00,0x00},{// >
0x08,0x04,0x02,0x01,0x02,0x04,0x08,0x00},{// ?
0x0E,0x11,0x01,0x02,0x04,0x00,0x04,0x00},{// @
0x0E,0x11,0x01,0x0D,0x15,0x15,0x0E,0x00},{// A
0x0E,0x11,0x11,0x11,0x1F,0x11,0x11,0x00},{// B
0x1E,0x09,0x09,0x0E,0x09,0x09,0x1E,0x00},{// C
0x0E,0x11,0x10,0x10,0x10,0x11,0x0E,0x00},{// D
0x1E,0x09,0x09,0x09,0x09,0x09,0x1E,0x00},{// E
0x1F,0x10,0x10,0x1F,0x10,0x10,0x1F,0x00},{// F
0x1F,0x10,0x10,0x1E,0x10,0x10,0x10,0x00},{// G
0x0E,0x11,0x10,0x13,0x11,0x11,0x0F,0x00},{// H
0x11,0x11,0x11,0x1F,0x11,0x11,0x11,0x00},{// I
0x0E,0x04,0x04,0x04,0x04,0x04,0x0E,0x00},{// J
0x07,0x02,0x02,0x02,0x02,0x12,0x0C,0x00},{// K
0x11,0x12,0x14,0x18,0x14,0x12,0x11,0x00},{// L
0x10,0x10,0x10,0x10,0x10,0x10,0x1F,0x00},{// M
0x11,0x1B,0x15,0x15,0x11,0x11,0x11,0x00},{// N
0x11,0x19,0x19,0x15,0x13,0x13,0x11,0x00},{// O
0x0E,0x11,0x11,0x11,0x11,0x11,0x0E,0x00},{// P
0x1E,0x11,0x11,0x1E,0x10,0x10,0x10,0x00},{// Q
0x0E,0x11,0x11,0x11,0x15,0x12,0x1D,0x00},{// R
0x1E,0x11,0x11,0x1E,0x14,0x12,0x11,0x00},{// S
0x0E,0x11,0x10,0x0E,0x01,0x11,0x0E,0x00},{// T
0x1F,0x04,0x04,0x04,0x04,0x04,0x04,0x00},{// U
0x11,0x11,0x11,0x11,0x11,0x11,0x0E,0x00},{// V
0x11,0x11,0x11,0x11,0x11,0x0A,0x04,0x00},{// W
0x11,0x11,0x11,0x15,0x15,0x1B,0x11,0x00},{// X
0x11,0x11,0x0A,0x04,0x0A,0x11,0x11,0x00},{// Y
0x11,0x11,0x11,0x0A,0x04,0x04,0x04,0x00},{// Z
0x1F,0x01,0x02,0x04,0x08,0x10,0x1F,0x00},{// [
0x0E,0x08,0x08,0x08,0x08,0x08,0x0E,0x00},{// \ **
0x00,0x10,0x08,0x04,0x02,0x01,0x00,0x00},{// ]
0x0E,0x02,0x02,0x02,0x02,0x02,0x0E,0x00},{// ^
0x04,0x0A,0x11,0x00,0x00,0x00,0x00,0x00},{// _
0x00,0x00,0x00,0x00,0x00,0x00,0x1F,0x00},{// `
0x10,0x08,0x04,0x00,0x00,0x00,0x00,0x00},{// a
0x00,0x00,0x0E,0x01,0x0F,0x11,0x0F,0x00},{// b
0x10,0x10,0x16,0x19,0x11,0x11,0x1E,0x00},{// c
0x00,0x00,0x0E,0x11,0x10,0x11,0x0E,0x00},{// d
0x01,0x01,0x0D,0x13,0x11,0x11,0x0F,0x00},{// e
0x00,0x00,0x0E,0x11,0x1F,0x10,0x0E,0x00},{// f
0x02,0x05,0x04,0x0E,0x04,0x04,0x04,0x00},{// g
0x00,0x0D,0x13,0x13,0x0D,0x01,0x0E,0x00},{// h
0x10,0x10,0x16,0x19,0x11,0x11,0x11,0x00},{// i
0x04,0x00,0x0C,0x04,0x04,0x04,0x0E,0x00},{// j
0x02,0x00,0x06,0x02,0x02,0x12,0x0C,0x00},{// k
0x08,0x08,0x09,0x0A,0x0C,0x0A,0x09,0x00},{// l
0x0C,0x04,0x04,0x04,0x04,0x04,0x0E,0x00},{// m
0x00,0x00,0x1A,0x15,0x15,0x15,0x15,0x00},{// n
0x00,0x00,0x16,0x19,0x11,0x11,0x11,0x00},{// o
0x00,0x00,0x0E,0x11,0x11,0x11,0x0E,0x00},{// p
0x00,0x16,0x19,0x19,0x16,0x10,0x10,0x00},{// q
0x00,0x0D,0x13,0x13,0x0D,0x01,0x01,0x00},{// r
0x00,0x00,0x16,0x19,0x10,0x10,0x10,0x00},{// s
0x00,0x00,0x0F,0x10,0x1E,0x01,0x1F,0x00},{// t
0x08,0x08,0x1C,0x08,0x08,0x09,0x06,0x00},{// u
0x00,0x00,0x12,0x12,0x12,0x12,0x0D,0x00},{// v
0x00,0x00,0x11,0x11,0x11,0x0A,0x04,0x00},{// w
0x00,0x00,0x11,0x11,0x15,0x15,0x0A,0x00},{// x
0x00,0x00,0x11,0x0A,0x04,0x0A,0x11,0x00},{// y
0x00,0x00,0x11,0x11,0x13,0x0D,0x01,0x0E},{// z
0x00,0x00,0x1F,0x02,0x04,0x08,0x1F,0x00}};

// Initialize the max7219 device
void initMatrix(void)
{
	writeRegister(matrixRegDecodeMode, 0x00);			// no decoding
	writeRegister(matrixRegIntensity, matrixIntensity);	// brightness intensity
	writeRegister(matrixRegScanLimit, 0x07);			// scan limit = 8 LEDs
	writeRegister(matrixRegShutdown, 0x01);				// power down=0, normal=1
	writeRegister(matrixRegDisplayTest, 0x00);			// no test display
}

// Clear the buffer and the matrix
void clear(void)
{
	int i;
	for(i = 0; i <= 7; i++)
	{
		buffer[i] = 0;
	}
	update();
}

// Push the buffer to the matrix
void update(void)
{
	int i;
	for(i = 0; i <= 7; i++)
	{
		writeRegister(i+1,buffer[i]);
	}
}

// Change the intensity of the lights in a sequence from lowest to highest, then back again.
// The intensity value is 4 bits, so it goes from 0 to 15.
void flash(void)
{
	uint8_t intensity;
	for(intensity = 0; intensity < 15; intensity++)
	{
		writeRegister(matrixRegIntensity,intensity);
		delay(SECOND/20);
	}
	for(intensity = 15; intensity > 0; intensity--)
	{
		writeRegister(matrixRegIntensity,intensity);
		delay(SECOND/20);
	}
	
	// Reset the register to its original state
	writeRegister(matrixRegIntensity,matrixIntensity);
}

// Get a letter from the characters array and place it in the buffer
void setLetter(uint8_t ascii)
{
	int i;
	for(i = 0; i <= 7; i++)
	{
		// Our collection starts with the 33rd item in the ascii table,
		// so we need to make sure the 'ascii' parameter gets the right character from the array
		// Therefore, subtract 32
		buffer[i] = characters[ascii-32][i];
	}
	update();
}

// Create a ball in the middle of the matrix
void setBall(void)
{
	buffer[0] = 0;	// 00000000
	buffer[1] = 0;	// 00000000
	buffer[2] = 24;	// 00011000
	buffer[3] = 60;	// 00111100
	buffer[4] = 60;	// 00111100
	buffer[5] = 24;	// 00011000
	buffer[6] = 0;	// 00000000
	buffer[7] = 0;	// 00000000
	
	update();
}

// Bitwise shift the whole buffer to the left
void shiftLeft(void)
{
	int i;
	for(i = 0; i <= 7; i++)
	{
		buffer[i] = buffer[i] << 1;
	}
	update();
}

// Bitwise shift the whole buffer to the right
void shiftRight(void)
{
	int i;
	for(i = 0; i <= 7; i++)
	{
		buffer[i] = buffer[i] >> 1;
	}
	update();
}

// Move all items in the array backwards
void shiftUp(void)
{
	int i;
	for(i = 0; i <= 7; i++)
	{
		buffer[i] = buffer[i + 1];
	}
	update();
}

// Move all items in the array forward
void shiftDown(void)
{
	int i;
	for (i = 7; i >= 0; i--)
	{        
		buffer[i] = buffer[i - 1];
	}
	update();
}

// Write something to SPI
void writeRegister(uint8_t address, uint8_t data)
{
	// The address and data bytes need to be combined into a 16 bit variable to send.
	// Address most significant, data least significant
	uint16_t combined = ((uint16_t)address << 8) | data;
    SPI_I2S_SendData16(SPI1,combined);
	// Sending messages too fast breaks everything, so wait a thousandth of a second.
	// This is the smallest delay that works for me.
	delay(SECOND/10000);
}
