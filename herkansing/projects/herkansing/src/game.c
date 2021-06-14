#include "game.h"

uint8_t paddle;
uint8_t ballX;
uint8_t ballY;
uint8_t ballD;

// The game init, runs once
void gameInit(void)
{
	// Initial game values
	paddle = 3;
	ballX = 4;
	ballY = 1;
	ballD = NORTHEAST;
	
	matrixClear();
	matrixUpdate();
}

// The game loop, runs at 20Hz
void gameUpdate(void)
{
	matrixClear();
	
	movePaddle();
	drawPaddle();
	
	moveBall();
	drawBall();
	
	matrixUpdate();
}

// Update the paddle location using a potentiometer
void movePaddle(void)
{
	
}

// Write the paddle to the display
void drawPaddle(void)
{
	buffer[0] = buffer[0] | (3 << (6 - paddle)); // 3 = 00000011
}

// Update the location of the ball
void moveBall(void)
{
	// West wall
	if(ballX == 0 && (ballD == NORTHWEST || ballD == SOUTHWEST))
	{
		// If the ball hits the paddle (ball AND paddle bit), continue
		if(buffer[ballX] & (1 << (7 - ballY)))
		{
			if(ballD == NORTHWEST) ballD = NORTHEAST;
			if(ballD == SOUTHWEST) ballD = SOUTHEAST;
		}
		
		// If the ball goes past the paddle, die
		else
		{
			
		}
	}
	
	// East wall
	if(ballX == 7 && ballD == NORTHEAST) ballD = NORTHWEST;
	if(ballX == 7 && ballD == SOUTHEAST) ballD = SOUTHWEST;
	
	// South wall
	if(ballY == 0 && ballD == SOUTHEAST) ballD = NORTHEAST;
	if(ballY == 0 && ballD == SOUTHWEST) ballD = NORTHWEST;
	
	// North wall
	if(ballY == 7 && ballD == NORTHEAST) ballD = SOUTHEAST;
	if(ballY == 7 && ballD == NORTHWEST) ballD = SOUTHWEST;
	
	switch(ballD)
	{
		case SOUTHEAST:
			ballX++;
			ballY--;
			break;
		case SOUTHWEST:
			ballX--;
			ballY--;
			break;
		case NORTHEAST:
			ballX++;
			ballY++;
			break;
		case NORTHWEST:
			ballX--;
			ballY++;
			break;
	}
}

// Write the ball to the display
void drawBall(void)
{
	buffer[ballX] = buffer[ballX] | (1 << (7 - ballY));
}
