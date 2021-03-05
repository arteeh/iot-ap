#include "include.h"

// Variable decleration
extern volatile char *rx_buffer;
extern volatile int head, tail;
extern volatile bool isBufferFullFlag;

//Returns isBufferFullFlag which is a boolean
bool isBufferFull(void){
	return isBufferFullFlag;
}

//Checks if the buffer is empty
bool isBufferEmpty(void){
	bool isEmpty = false;
	// If tail and head are on the same location, and isBufferFull flag is not true, we can be sure that the buffer is empty. 
	if(head == tail && isBufferFullFlag != true){
		isEmpty = true;
	}
	return isEmpty;
}


// Append given character to the tail
void addToBuffer(char data){
	if(isBufferFull()) {
		// Enable the blue LED on the board when the buffer is full
		GPIOC->BSRR = GPIO_BSRR_BS_8;
	} else {
		// Put data inside the head index
		rx_buffer[head] = data;
		//Do a modulus expression to give head a new value. This because rx_buffer is a ring buffer
		head = (head + 1) % BUFFER_SIZE;
		if(head == tail){
			isBufferFullFlag = true;
		}
	}
} 

// Return read character and move tail location
char readFromBuffer(void){
	char tempData;
	if(isBufferEmpty()){
		// Return null if buffer is empty, and there is nothing to read.
		return NULL;
	} else {
		// Give tempData the value on the tail
		tempData = rx_buffer[tail];
		// Do a modulus expression to give tail a new value. This because rx_buffr is a ring buffer
		tail = (tail + 1) % BUFFER_SIZE;
		isBufferFullFlag = false;
	}
	return tempData;
}
