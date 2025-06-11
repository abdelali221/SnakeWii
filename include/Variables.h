#ifndef _VARIABLES_H_
#define _VARIABLES_H_
#include "WiiLibs.h"
#define HOR_OFFSET 12
#define VER_OFFSET 1
#define VER "1.7"

extern bool Resume;
extern bool GenBall;
extern bool Start;
extern bool PressedButton;
extern bool BallEaten;
extern bool doPause;
extern bool ingame;
extern bool WPADType;
extern uint16_t Speed;

extern const uint8_t ROWS;
extern const uint8_t COLS;

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