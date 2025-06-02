#include "Variables.h"

bool Resume = false;
bool GenBall = true;
bool Start = false;
bool PressedButton = false;
bool BallEaten = false;
bool doPause = false;

const uint8_t ROWS = 20 + VER_OFFSET;
const uint8_t COLS = 50 + HOR_OFFSET;

uint8_t SnakeX = (COLS/2) + 4;
uint8_t SnakeY = ROWS/2;
uint8_t ANSBallX = 0;
uint8_t ANSBallY = 0;
uint8_t BallX = 0;
uint8_t BallY = 0;
int VSnakeX = 0;
int VSnakeY = 0;
uint8_t Lives = 3;
uint16_t Score = 0;
uint16_t SnakeLength = 2;
uint16_t Speed;
uint8_t counter = 0;

uint8_t SnakePOSbuffer[1920][2];