#ifndef GAME_H
#define GAME_H

#include "display.h"

// How long the game should be, horizontally.
// 7 for the max7219 size, 8 for fun gameplay.
#define GAME_WIDTH		7

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

extern uint8_t paddle;

void gameInit(void);
void gameUpdate(void);
void movePaddle(void);
void moveBall(void);
void faster(void);

#endif // GAME_H
