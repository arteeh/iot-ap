#include "game.h"

uint8_t paddle;
uint16_t speed;

struct ball
{
	uint8_t x;
	uint8_t y;
	enum direction d;
} ball;

// Reset the game, runs once initially and on blue button press
void gameInit(void)
{	
	// Initial game values
	ball.x = 2;
	ball.y = 5;
	ball.d = northeast;
	
	// Reset the speed
	speed = SPEED_INITIAL;
	TIM2->ARR = speed - 1;
}

// Write the paddle to the display
void movePaddle(void)
{
	buffer[0] = (3 << (6 - paddle)); // 3 = 00000011
	
	// Draw the buffer to the screen
	displayUpdate();
}

// Update the location of the ball, account for collision
void moveBall(void)
{
	int i;

	// West wall
	if(ball.x == 0)
	{
		switch(checkHit())
		{
			case hit:
				// Bounce off the paddle
				if(ball.d == northwest) ball.d = northeast;
				if(ball.d == southwest) ball.d = southeast;
				// Speed up the ball
				faster();
				break;
			case west:
				// If the ball goes past the paddle, die
				TIM_Cmd(TIM2, DISABLE);
				break;
			case east:
				break;
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
	
	// Make sure the previous ball is gone
	for(i = 1; i <= 7; i++)
	{
		if(i != ball.x) buffer[i] = 0;
	}
	
	// Draw the ball to the screen
	buffer[ball.x] |= (1 << (7 - ball.y));
	
	// Draw the buffer to the screen
	displayUpdate();
}

// Make the ball go faster
void faster(void)
{
	// Set the autoreload register for timer 2
	TIM2->ARR = speed - 1;
	
	// Slowly gain speed every time this function is called.
	// Don't go faster than SPEED_MAX.
	if(speed > SPEED_MAX) speed = speed * 4 / 5;
}

// Check if the ball hits the paddle
enum hit checkHit(void)
{
	if(ball.x <= 0)
	{
		// The ball collides with the paddle
		if(buffer[0] & (1 << (7 - ball.y))) return hit;
		// The ball has gone past the paddle
		else return west;
	}
	// The ball is still in the game
	else return east;
}

// Beep when the ball hits the paddle
void beepHit(void)
{
	// Set timer to 1000Hz
	TIM3->ARR = 48000 - 1;
	TIM3->PSC = 0;
	
	// Turn on the speaker
	TIM_SetCompare2(TIM3, 256);
}

// Beep when the ball passes the paddle
void beepLose(void)
{
	// Set timer to 400Hz
	TIM3->ARR = 60000 - 1;
	TIM3->PSC = 1;
	
	// Turn on the speaker
	TIM_SetCompare2(TIM3, 256);
}
