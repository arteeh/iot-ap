#ifndef GAME_H
#define GAME_H

#include "graphics.h"

#define SOUTHEAST 0
#define SOUTHWEST 1
#define NORTHEAST 2
#define NORTHWEST 3

void gameInit(void);
void gameUpdate(void);
void drawPaddle(void);
void movePaddle(void);
void drawBall(void);
void moveBall(void);

#endif // GAME_H
