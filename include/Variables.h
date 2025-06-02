#ifndef _VARIABLES_H_
#define _VARIABLES_H_
#include "WiiLibs.h"
#define ROWS 20 + VER_OFFSET
#define COLS 50 + HOR_OFFSET
#define HOR_OFFSET 10
#define VER_OFFSET 0

extern bool Resume;
extern bool GenBall;
extern bool Start;
extern bool PressedButton;
extern bool BallEaten;
extern bool doPause;

extern uint8_t BallX, BallY, ANSBallX, ANSBallY;
extern uint8_t SnakeX;
extern uint8_t SnakeY;
extern int VSnakeX;
extern int VSnakeY;
extern uint8_t Lives;
extern uint16_t Score;
extern uint16_t SnakeLength;
extern uint8_t counter;
extern uint8_t SnakePOSbuffer[1920][2];

#endif