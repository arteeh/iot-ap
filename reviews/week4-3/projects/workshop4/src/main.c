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

//change these values for different format.
#define SHIFT_AMOUNT 16 // 16.16
#define SHIFT_MASK ((1 << SHIFT_AMOUNT) - 1) // make 16 lsb bits high = 2^16

#define ROUNDFIX  0.055

//to make sure roundings go right we have to add 00.55
//this is  (0.055 * 2^16)
#define FixRoundings(a) (fixed)(a + (ROUNDFIX * SHIFT_MASK))

//show the integar bits, so the MSB-1 till bit 16
// a*10 later / 10 because otherwise -19 will get rounded to -20, see testcase 8
#define ShowIntegar(a) (((a * 10) >> SHIFT_AMOUNT) / 10)

//first make sure you only have the 16 LSB, so the fraction part
//this number times * 10 / 1 << shift_amount gives the fractional part
#define ShowFragment(a) (((a & SHIFT_MASK)*10) / (1 << SHIFT_AMOUNT))


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
//32 bits fixed point
typedef int32_t fixed;

//convert float to fixed
#define float_to_fixed(a)   (fixed)((a) * (1 << SHIFT_AMOUNT))

//calculate the average this can be done normally
fixed calc_average( fixed* array, int8_t lenght) {

	int8_t hour;
  fixed sum = 0;
  for ( hour=0; hour<lenght; hour++) {
    sum+= *array++;
  }
	//now add the 0,055 so the roundings will go the right way when you want to showintegar/fragment
  return FixRoundings(sum/lenght);
}

//to string
void to_string( fixed number ) {
	sprintf(buffer, "%d.%1d", 
	ShowIntegar(number), //show the integar
	ShowFragment(number)); //show the fragment
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
