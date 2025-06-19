#include "Utils.h"
#include "UI.h"
#include "Audio.h"
#include "SnakeUtils.h"
#include "WiiLibs.h"
#include "Variables.h"
#include "Video.h"
#include "Input.h"


void CheckController() {
	int pressed = CheckWPAD();

	switch (pressed)
	{	
		case UP:
			if (!PressedButton && (!Start || VSnakeX != 0)) 
			{
				PressedButton = true;
				Start = true;
				VSnakeY = -1;
				VSnakeX = 0;
			}
		break;

		case DOWN:
			if (!PressedButton && (!Start || VSnakeX != 0)) 
			{
				PressedButton = true;
				Start = true;
				VSnakeY = 1;
				VSnakeX = 0;
			}
		break;

		case LEFT:
			if (!PressedButton && (!Start || VSnakeY != 0)) 
			{
				PressedButton = true;
				Start = true;
				VSnakeX = -1;
				VSnakeY = 0;
			}
		break;

		case RIGHT:
			if (!PressedButton && (!Start || VSnakeY != 0)) 
			{
				PressedButton = true;
				Start = true;
				VSnakeX = 1;
				VSnakeY = 0;
			}
		break;
		
		case PLUS:
			doPause = true;
		break;

		default:
		break;
	}
}

void sleep(u32 delay) {
	u32 ticks = delay/20;
	u32 start = 0;
	while (start < ticks) {
		if (ingame) {
			CheckController();
			if (doPause) return;
		}
		start++;
		VIDEO_WaitVSync();
	}
}

void PrintGameStats() {
	POSCursor(1, ROWS + 2);
	textprint(" Score : ");
	POSCursor(10, ROWS + 2);
	valprint(Score);
	POSCursor(1, ROWS + 4);
	textprint(" Lives : ");
	POSCursor(10, ROWS + 4);
	valprint(Lives);
}

bool Loose() {
	GenBall = true;
	Start = false;
	for (size_t i = 1; i < 599; i++) {
		SnakePOSbuffer[i][0] = 0;
		SnakePOSbuffer[i][1] = 0;
	}
	SnakeX = (COLS/2) + 4 + VER_OFFSET;
	SnakeY = ROWS/2;
	VSnakeX = 0;
	VSnakeY = 0;
	SnakeLength = 2;
	ClearScreen();
	ingame = false;
	if (Lives == 0) {
		Play(DIED);
		sleep(4000);
		GameOver();
		return true;
	} else {
		Play(LOST);
		sleep(2000);
		RenderBorders(false, true);
		Lives--;
		ingame = true;
		return false;
	}
}

void RunGame() {
	if (!ingame)
	{
		ingame = true;
		if (!doPause)
			RenderBorders(true, true);
	}
	while (1)
	{	
		RenderBorders(false, false);
		if (counter < 4*(1000/Speed)) {
			counter++;
		} else {
			counter = 0;
		}
		if (counter == 3*(1000/Speed) && GenBall) {
			GenerateBall();
			GenBall = false;
		}
		PressedButton = false;
		sleep(Speed);
		PrintGameStats();
		ManageSnakePos();
		RenderSnake();
		if (!BallEaten && !GenBall && (BallX != 0 && BallY != 0)) DrawBall(BallX, BallY);
		VIDEO_WaitVSync();
		if (!ingame) {
			doPause = false;
			return;
		}
		if (doPause) {
			Pause();
			if (!ingame) {
				return;
			}
		}
		RenderBuffer();
	}
}