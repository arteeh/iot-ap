//#include "stm32f0xx.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "usart.h"

void assert_strequal( int test, char* b, char* expected, char* message );

// Comment the following line to test your fixed point solution instead of the given example
#define RUN_FLOAT 1

// For printing
char buffer[20];

uint8_t passed;

#ifdef RUN_FLOAT

// Note that still one testcase fails because float is not accurate enough!
typedef float fixed;

// More precision floatint point type passes all tests
// Typedef double fixed;

// Float does not need conversion
#define float_to_fixed(a) (a)

struct testcase
{
	uint8_t aantalgetallen;
	float invoer[5];
	char* verwachtte_uitvoer;
	char* melding;
};

const int nrofTestcases = 9;

struct testcase testcases[] = {
	//nrvalues	input				output		test
	{1,		{1, 0, 0, 0, 0},		"1.0",		"simple"},
	{1,		{0.1, 0, 0, 0, 0},		"0.1",		"simple"},
	{2,		{36.3, 36.3, 0, 0, 0},		"36.3",		"average"},
	{2,		{36.3, 36.4, 0, 0, 0},		"36.4",		"average with rounding"},
	{3,		{36.3, 36.4, 0, 0, 0},		"24.2",		"average with rounding"},
	{5,		{110, 110, 110, 110, 110},	"110.0",	"overflow"},
	{5,		{-20, -20, -20, -20, -20},	"-20.0",	"underflow"},
	{2,		{-20, -19, 0, 0, 0},		"-19.5",	"rounding negative values"},
	{2,		{-20, 20, 0, 0, 0},		"0.0",		"negative + positive number "}
};

fixed calc_average( fixed* array, int8_t length)
{
	int8_t hour;
	fixed sum = 0;
	for ( hour=0; hour < length; hour++)
	{
		sum += *array++;
	}
	
	// Float does not need rounding
	return sum / length;
}

void to_string(fixed number)
{
	// Printing takes care of rounding
	sprintf(buffer, "%.1f", number);
}

#else
	
// Your fixed point arithmetic code goes here

// Choose the correct type for fixed
// Here change 'float' to the correct integer type
typedef float fixed;

// Make the correct conversion macro
// Change this
#define float_to_fixed(a) (a)

// Implement this code yourself
fixed calc_average(fixed* array, int8_t length)
{
	// your code goes here
	return 0;
}

// Implement this code yourself
void to_string(fixed number)
{
	sprintf(buffer,
		"%d.%1d",
		0,	//compute the integer part
		0);	//compute the fractional part
}

#endif // RUN_FLOAT

void assert_strequal(int test, char* b, char* expected, char* message)
{
	USART_putstr("TEST: ");
	USART_putint(test+1);
	
	// Check the first 10 digits for equality
	if (strncmp(b, expected, 10) != 0)
	{
		USART_putstr(" FAIL: ");
		USART_putstr(b);
		USART_putstr(" != ");
		USART_putstr(expected);
		USART_putstr(" ");
		USART_putstr(message);
	}
	else
	{
		USART_putstr(" PASS: ");
		USART_putstr(b);
		USART_putstr(" = ");
		USART_putstr(expected);
		passed++;
	}
	
	USART_putstr("\n");
}

int main(void)
{
	uint8_t test;
	uint8_t j;
	
	fixed avg;
	fixed hourly_temperatures[24];

	passed = 0;
	
	USART_init();
	USART_clearscreen();
	
#ifdef RUN_FLOAT
	USART_putstr("This is testcode for floating point arithmetic\n");
#else
	USART_putstr("This is testcode for fixed point arithmetic\n");
#endif

	USART_putstr("The output of the testcases is\n\n");

	for (test=0; test<nrofTestcases; test++)
	{
		// Do each test
		struct testcase thistest = testcases[test];

		// Fill values array
		for (j=0; j<thistest.aantalgetallen; j++)
		{
			hourly_temperatures[j] = float_to_fixed(thistest.invoer[j]);
		}

		avg = calc_average(hourly_temperatures, thistest.aantalgetallen);
		to_string(avg);
		assert_strequal(test, buffer, thistest.verwachtte_uitvoer, thistest.melding);
	}

	USART_putstr("\nYou passed ");
	USART_putint(passed);
	USART_putstr(" of the tests\n");
	USART_putstr("Ready");

	while(1);
}

