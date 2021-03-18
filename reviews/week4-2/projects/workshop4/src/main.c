/******************************************************************************
 * File           : Main program
 * klas		     		: KlasB
 *****************************************************************************/
//#include "stm32f0xx.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "usart.h"

#define SHIFT_AMOUNT (4) // 2^4 = 15
#define SHIFT_MASK ((1 << SHIFT_AMOUNT) - 1) // 15 (all LSB set, all MSB clear)
#define MIN_MASK (0x8000)
#define NEGATIVE_ROUND_OFF_ADJUST (13)
#define ROUND_OFF_ADJUST (1)
#define SHIFT_AMOUNT_DECIMAL (15.0) //2^4 = 15


void assert_strequal( int test, char* b, char* expected, char* message );


//comment the following line to test your fixed point solution instead of the given example
//#define RUN_FLOAT 1


char      buffer[20];  //for printing
uint8_t   passed;



#ifdef RUN_FLOAT

typedef   float  fixed;     //note that still one testcase fails because float is not accurate enough!
//typedef   double  fixed;  //more precision floatint point type passes all tests

#define float_to_fixed(a)   (a)   //float does not need conversion

fixed calc_average( fixed* array, int8_t lenght) {
  int8_t hour;
  fixed sum = 0;
  for ( hour=0; hour<lenght; hour++) {
    sum += *array++;
  }
  return sum / lenght;   //float does not need rounding
}

void to_string( fixed number )
{
  sprintf(buffer, "%.1f", number);  //printing takes care of rounding
}

#else
	
typedef   int16_t  fixed;   

#define float_to_fixed(a)   ((fixed) (((a)*SHIFT_AMOUNT_DECIMAL)))   

fixed calc_average( fixed* array, int8_t lenght) {
	
	fixed result = 0;
  int32_t sum = 0;
	int8_t i;
  for (i = 0; i<lenght; i++) {
  sum += (array[i]) << SHIFT_AMOUNT;
  }
	
	result = (sum / lenght)/SHIFT_AMOUNT_DECIMAL;
	
	//if negative adjust fractional part
	if(result & MIN_MASK){
		if((result & SHIFT_MASK) > 1){
			result += NEGATIVE_ROUND_OFF_ADJUST;
		}
	//small adjust for the correct round off
	}else if((result & SHIFT_MASK) > 1){
			result -= ROUND_OFF_ADJUST;
	}
	
	return result;
}

void to_string( fixed number ) {
	sprintf(buffer, "%d.%1d",
	        number >> SHIFT_AMOUNT, //compute the integer part
				number & SHIFT_MASK); //compute the fractional part
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
		USART_putstr( " FAIL: " );
		USART_putstr( b );
		USART_putstr( " != " );
		USART_putstr( expected );
		USART_putstr( " " );
		USART_putstr( message );
	}
	else
	{
		USART_putstr( " PASS: " );
		USART_putstr( b );
		USART_putstr( " = " );
		USART_putstr( expected );
		passed++;
	}
  USART_putstr( "\n" );
}
