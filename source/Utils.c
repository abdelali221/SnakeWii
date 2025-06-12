#include "Utils.h"
#include "UI.h"
#include "Audio.h"
#include "SnakeUtils.h"
#include "WiiLibs.h"
#include "Variables.h"
#include "WiiVT.h"
#include "Input.h"

void VideoInit() {
	static void *xfb = NULL;
	static GXRModeObj *Vmode = NULL;

	VIDEO_Init();
	ASND_Init();
	Vmode = VIDEO_GetPreferredMode(NULL);

	xfb = MEM_K0_TO_K1(SYS_AllocateFramebuffer(Vmode));

	console_init(xfb, 20, 20, Vmode->fbWidth, Vmode->xfbHeight, Vmode->fbWidth * VI_DISPLAY_PIX_SZ);

	VIDEO_Configure(Vmode);

	VIDEO_SetNextFramebuffer(xfb);

	VIDEO_ClearFrameBuffer(Vmode, xfb, COLOR_BLACK);

	VIDEO_SetBlack(false);

	VIDEO_Flush();

	VIDEO_WaitVSync();
	if(Vmode->viTVMode&VI_NON_INTERLACE) VIDEO_WaitVSync();
}

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
			if (ingame)
			{
				doPause = true;
				Pause();
			}
		break;

		default:
		break;
	}
}

void RenderBorders(bool DELAY, bool PLAYSOUND) {
	ingame = false;
	for (size_t Y = VER_OFFSET; Y <= ROWS; Y++) {
  
		for (size_t X = HOR_OFFSET; X <= COLS; X++) {
  
			if ( ( (X == HOR_OFFSET || X == COLS) && (Y >= VER_OFFSET && Y <= ROWS) )|| (Y == VER_OFFSET || Y == ROWS)) {
  				POSCursor(X, Y);
				if (DELAY) {
  					sleep(20);
				}
		  		printf("#");
  			}
		}
	}
	if (PLAYSOUND) {
		Play(START);
	}
	ingame = true;
}

void sleep(u32 delay) {
	u32 ticks = delay/20;
	u32 start = 0;
	while (start < ticks) {
		CheckController();
		if (doPause && ingame) {
			return;
		}
		start++;
		VIDEO_WaitVSync();
	}
}

void PrintGameStats() {
	POSCursor(0, ROWS + 2);
	printf(" Score : %d \n", Score);
	printf(" Lives : %d ", Lives);
}

void GenerateBall() {
	for (size_t i = 1; i <= SnakeLength; i++) {
		while (BallX < HOR_OFFSET + 1 || BallX > COLS || BallY < VER_OFFSET + 1 || BallY > ROWS || (BallX == SnakePOSbuffer[i][0] && BallY == SnakePOSbuffer[i][1]) || (BallX == ANSBallX && BallY == ANSBallY)) {
			BallX = HOR_OFFSET + 1 + rand() % (COLS - HOR_OFFSET - 1);
			BallY = VER_OFFSET + 1 + rand() % (ROWS - VER_OFFSET - 1);
		}
	}
	BallEaten = false;
	ANSBallX = BallX;
	ANSBallY = BallY;
	POSCursor(BallX, BallY);
	printf("O");
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
		if (doPause)
			Pause();

		if (!ingame)
			return;

		PrintGameStats();
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
		ManageSnakePos();
		RenderSnake();
		VIDEO_WaitVSync();
	}
}