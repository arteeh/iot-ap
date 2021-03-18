/******************************************************************************
 * File           : Main program
 * Author         : VNP02
 *****************************************************************************/
//#include "stm32f0xx.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "usart.h"

void assert_strequal( int test, char* b, char* expected, char* message );


//comment the following line to test your fixed point solution instead of the given example
//#define RUN_FLOAT 1


char      buffer[20];  //for printing
uint8_t   passed;



#ifdef RUN_FLOAT

//typedef   float  fixed;     //note that still one testcase fails because float is not accurate enough!
typedef   double  fixed;  //more precision floatint point type passes all tests

#define float_to_fixed(a)   (a)   //float does not need conversion

fixed calc_average( fixed* array, int8_t lenght) {
  int8_t hour;
  fixed sum = 0;
  for ( hour=0; hour<lenght; hour++) {
    sum += *array++;
		//sum += 1.1 x *array++;
  }
  return sum / lenght;   //float does not need rounding
}

void to_string( fixed number )
{
  sprintf(buffer, "%.1f", number);  //printing takes care of rounding
}

#else
	
//your fixed point arithmetic code goes here
#define SHIFT_AMOUNT	21 // 1.10.21 fixed point getal
#define SHIFT_MASK		((1 << SHIFT_AMOUNT) - 1)
// choose the correct type for fixed
typedef   int32_t  fixed;   //here change 'float' to the correct integer type

// make the correct conversion macro
#define float_to_fixed(a)   ((fixed)((a) * (1 << SHIFT_AMOUNT)))	// 2 ^ 21

// implement this code yourself
fixed calc_average( fixed* array, int8_t length){
	uint8_t i;
	fixed sum = 0;
	for (i = 0; i < length; i++ ){
		sum += (array[i]);
		//sum += (float_to_fixed(1.1) * (array[i])) >> SHIFT_AMOUNT;
	}
	sum /= length;
	return sum;
}

// implement this code yourself
void to_string(fixed number){
	// Geef het juiste nummer terug en pas afronding toe
	sprintf(buffer, "%d.%1d", (number + (1 << (SHIFT_AMOUNT) - 1)) >> SHIFT_AMOUNT, (((number & SHIFT_MASK) * 10) + (1 << (SHIFT_AMOUNT - 1))) / (1 << SHIFT_AMOUNT));
}

#endif // RUN_FLOAT

struct testcase
{
	  uint8_t aantalgetallen;
	  float   invoer[5];
	  char*   verwachtte_uitvoer;
	  char*   melding;
};

const int nrofTestcases = 9;

struct testcase testcases[] = {
  //nrvalues       input              output   test
      { 1, {1, 0, 0, 0, 0},           "1.0",   "simple" },
      { 1, {0.1, 0, 0, 0, 0},         "0.1",   "simple" },
      { 2, {36.3, 36.3, 0, 0, 0},     "36.3",  "average" },
      { 2, {36.3, 36.4, 0, 0, 0},     "36.4",  "average with rounding" },
      { 3, {36.3, 36.4, 0, 0, 0},     "24.2",  "average with rounding" },
      { 5, {110, 110, 110, 110, 110}, "110.0", "overflow" },
      { 5, {-20, -20, -20, -20, -20}, "-20.0", "underflow" },
      { 2, {-20, -19, 0, 0, 0},       "-19.5", "rounding negative values" },
      { 2, {-20, 20, 0, 0, 0},        "0.0",   "negative + positive number " }
};

// ----------------------------------------------------------------------------
// Main
// ----------------------------------------------------------------------------
int main(void)
{
  uint8_t test, j;
  fixed   avg;
  fixed   hourly_temperatures[ 24 ];

  passed = 0;
	
  USART_init();
  USART_clearscreen();
#ifdef RUN_FLOAT
  USART_putstr("This is testcode for floating point arithmetic\n");
#else
  USART_putstr("This is testcode for fixed point arithmetic\n");
#endif
  USART_putstr("The output of the testcases is\n\n");

  for ( test=0; test<nrofTestcases; test++)
  {
	 //do each test
	 struct testcase thistest = testcases[test];

	 //fill values array
	 for (j=0; j<thistest.aantalgetallen; j++)
	 {
        hourly_temperatures[j] = float_to_fixed(thistest.invoer[j]);
	 }

     avg = calc_average( hourly_temperatures, thistest.aantalgetallen );
     to_string( avg );
     assert_strequal( test, buffer, thistest.verwachtte_uitvoer, thistest.melding );
  }

  USART_putstr( "\nYou passed " );
  USART_putint( passed );
  USART_putstr( " of the tests\n" );
  USART_putstr("Ready");

  //ready
  while(1);
}


void assert_strequal( int test, char* b, char* expected, char* message )
{
	USART_putstr( "TEST: " );
	USART_putint( test+1 );
	if ( strncmp( b, expected, 10 ) != 0 )   //check the first 10 digits for equality
	{
		USART_putstr( " \033[31mFAIL:\033[0m " );
		USART_putstr( b );
		USART_putstr( " != " );
		USART_putstr( expected );
		USART_putstr( " " );
		USART_putstr( message );
	}
	else
	{
		USART_putstr( " \033[32mPASS:\033[0m " );
		USART_putstr( b );
		USART_putstr( " = " );
		USART_putstr( expected );
		passed++;
	}
  USART_putstr( "\n" );
}
