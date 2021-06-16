#ifndef GAME_H
#define GAME_H

#include <stdbool.h>
#include "display.h"

// How long the game should be, horizontally.
// 7 for the max7219 size, 8 for fun gameplay.
#define GAME_WIDTH		8

// This sets the autoreload register for timer 2.
// Smaller number means faster ball.
#define SPEED_INITIAL	250

// The highest speed that's allowed
#define SPEED_MAX		40

enum direction
{
	southeast,
	southwest,
	northeast,
	northwest
};

enum hit
{
	hit,	// The ball hit the paddle
	west,	// The ball has gone past the paddle
	east	// The ball is still in the game
};

// Paddle should be extern because it's set in stm32f0xx_it.c
extern uint8_t paddle;

void gameInit(void);
void movePaddle(void);
void moveBall(void);
void faster(void);
enum hit checkHit(void);
void beepHit(void);
void beepLose(void);

#endif // GAME_H
