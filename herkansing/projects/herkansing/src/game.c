#include "game.h"

uint8_t paddle = 1;

struct ball
{
	uint8_t x;
	uint8_t y;
	enum direction d;
} ball;

// The game init, runs once
void gameInit(void)
{	
	// Initial game values
	ball.x = 2;
	ball.y = 5;
	ball.d = northeast;
	displayClear();
	displayUpdate();
}

// The game loop
void gameUpdate(void)
{
	displayClear();
	moveBall();
	movePaddle();
	displayUpdate();
}

// Write the paddle to the display
void movePaddle(void)
{
	//buffer[0] = buffer[0] | (3 << (6 - paddle)); // 3 = 00000011
	buffer[0] = (3 << (6 - paddle)); // 3 = 00000011
}

// Update the location of the ball, account for collision
void moveBall(void)
{
	// West wall
	if(ball.x == 0)
	{
		// If the ball hits the paddle (ball byte AND paddle byte), continue
		if(buffer[0] & (1 << (7 - ball.y)))
		{
			if(ball.d == northwest) ball.d = northeast;
			if(ball.d == southwest) ball.d = southeast;
			
			faster();
		}
		
		// If the ball goes past the paddle, die
		else
		{
			TIM_Cmd(TIM2, DISABLE);
		}
	}
	
	// East wall
	if(ball.x == GAME_WIDTH && ball.d == northeast) ball.d = northwest;
	else if(ball.x == GAME_WIDTH && ball.d == southeast) ball.d = southwest;
	
	// South wall
	if(ball.y == 0 && ball.d == southeast) ball.d = northeast;
	else if(ball.y == 0 && ball.d == southwest) ball.d = northwest;
	
	// North wall
	if(ball.y == 7 && ball.d == northeast) ball.d = southeast;
	else if(ball.y == 7 && ball.d == northwest) ball.d = southwest;
	
	// Update the coordinates
	switch(ball.d)
	{
		case southeast:
			ball.x++;
			ball.y--;
			break;
		case southwest:
			ball.x--;
			ball.y--;
			break;
		case northeast:
			ball.x++;
			ball.y++;
			break;
		case northwest:
			ball.x--;
			ball.y++;
			break;
	}
	
	// Draw the ball to the screen
	buffer[ball.x] = buffer[ball.x] | (1 << (7 - ball.y));
}

// Make the ball go faster
void faster(void)
{
	static uint16_t speed = SPEED_INITIAL;
	
	// Set the autoreload register for timer 2
	TIM2->ARR = speed - 1;
	
	if(speed > SPEED_MAX) speed = speed * 4 / 5;
}
