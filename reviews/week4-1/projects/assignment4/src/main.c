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

// choose the correct type for fixed
typedef  int32_t fixed;   //here	 change 'float' to the correct integer type

//your fixed point arithmetic code goes here

//0110 1010 010 . 1 0110 1101 1010 0101 0011
#define SCALE 21 //21 fractional bits and 10 integer bits
#define FRACTIONDIVISOR (1 << SCALE) // 2^16 (65536)
#define FRACTIONMASK (FRACTIONDIVISOR - 1) //mask to show fractional part
#define ONEDIGIT 10 // integer to make sure we round to 1 digit afet decimal point
#define ROUNDING FRACTIONDIVISOR/20 //define for correct rounding
#define NEGATIVEVALUE ((number >> SCALE) & (1<<(SCALE -1))) //to check for negative values

// macro for arithmetic operations
#define float_to_fixed(a)   ((fixed)((a)*(1 << SCALE)))  
#define fixed_to_float(a) ((float)(a)/(1 << SCALE))
#define fractional_part(a) (((int64_t)(((a) + ROUNDING) & FRACTIONMASK) * ONEDIGIT) / FRACTIONDIVISOR)  //macro for computing fractional part 
#define whole_part(a) ((a) >> SCALE) //macro for computing the integer part
#define int_to_fixed(a) ((fixed) ((a) << SCALE)) 
#define div_fixed(a,b) ((fixed) ((((int64_t) (a) << SCALE) / (b))))

//functions 
fixed calc_average( fixed* array, int8_t lenght) {
	
	uint8_t i; //index
	fixed sum = 0; 
	fixed average;
	
	for(i = 0; i < lenght; i++) { //for each temperature 
		sum +=  *array++; //add it to the sum
	}
	
	//int_to_fixed to do the correct arithmetic operation
	average = div_fixed(sum, int_to_fixed(lenght)); //divide the sum with the length of the array and store it in the average
	
	return average;
}

// implement this code yourself
void to_string( fixed number ) { 
	
	//workaround for a problem that occurs with negative rounding, not the best way to do this but it works :)
	if(fractional_part(number) && NEGATIVEVALUE ) { //check if there is a fractional part and it is a negative number
			sprintf(buffer, "%d.%lld",
	        whole_part(number) + 1, //if there is a negative value with a fractional part add 1 to the integer part to make sure we print the right value
					fractional_part(number)); //fractional part stays the same
	} else { //all other values will print the right way
			sprintf(buffer, "%d.%lld",
	        whole_part(number), //compute the integer part
					fractional_part(number)); //compute the fractional part
	}
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
