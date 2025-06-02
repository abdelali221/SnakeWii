#include "SnakeUtils.h"
#include "WiiVT.h"
#include "WiiLibs.h"
#include "Variables.h"
#include "Utils.h"
#include "OGGFiles.h"
#include "sample_ogg.h"

bool Invincible = false;

static void *xfb = NULL;
static GXRModeObj *Vmode = NULL;

void SystemInit() {

	VIDEO_Init();

	WPAD_Init();

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

void GameOver() {
	POSCursor(30, 10);
	printf("Game Over!");
	POSCursor(27, 12);
	printf("Your Score : %d", Score);
	POSCursor(14, 14);
	printf("Press HOME to exit or A to restart the game");
	while (true) {
		WPAD_ScanPads();
		u32 pressed = WPAD_ButtonsDown(0);
		if (pressed & WPAD_BUTTON_HOME) {
			exit(0);
		} else if (pressed & WPAD_BUTTON_A) {
			printf("\x1b[2J");
			DifficultySelect();
			RenderBorders(true, true);
			return;
		}
	}

}

void Loose() {
	printf("\x1b[2J");
	GenBall = true;
	Start = false;
	for (size_t i = 1; i < 599; i++) {
		SnakePOSbuffer[i][0] = 0;
		SnakePOSbuffer[i][1] = 0;
	}
	SnakeX = (COLS/2) + 4;
	SnakeY = ROWS/2;
	VSnakeX = 0;
	VSnakeY = 0;
	SnakeLength = 2;
	 
	if (Lives > 0) {
		PlayOgg(lost_ogg, lost_ogg_size,  0, OGG_ONE_TIME);
		sleep(2000);
		RenderBorders(false, true);
		Lives--;
	} else {
		PlayOgg(died_ogg, died_ogg_size,  0, OGG_ONE_TIME);
		sleep(4000);
		GameOver();
		Lives = 3;
		Score = 0;
	}
}

void ManageSnakePos() {
	CheckController();
	if (!Invincible && (SnakeX < HOR_OFFSET + 1 || SnakeX > COLS - 1 || SnakeY < VER_OFFSET + 1 || SnakeY > ROWS - 1)) {
		Loose();
	}
	if (SnakeLength > 4) {
		for (size_t i = 4; i < SnakeLength + 1; i++) {
			if (SnakeX == SnakePOSbuffer[i][0] && SnakeY == SnakePOSbuffer[i][1]) {
				Loose();
			}
		}
	}
		
	if (SnakeX == BallX && SnakeY == BallY && !BallEaten) {
		 
		PlayOgg(increase_ogg, increase_ogg_size,  0, OGG_ONE_TIME);
		Score++;
		SnakeLength++;
		BallEaten = true;
		GenBall = true;
	}
	SnakeX = SnakeX + VSnakeX;
	SnakeY = SnakeY + VSnakeY;
}

void PrintGameStats() {
	POSCursor(0, ROWS + 2);
	printf(" Score : %d \n", Score);
	printf(" Lives : %d ", Lives);
}

void Pause() {
	printf("\x1b[2J");
	POSCursor(30, 10);
	printf("Paused!");
	POSCursor(25, 12);
	printf("Press + to resume...");
	PlayOgg(pause_ogg, pause_ogg_size,  0, OGG_ONE_TIME);
	while (1) {
		WPAD_ScanPads();
		u32 pressed = WPAD_ButtonsDown(0);
		if (pressed & WPAD_BUTTON_PLUS) {
			PlayOgg(resume_ogg, resume_ogg_size, 0, OGG_ONE_TIME);
			printf("\x1b[2J");
			RenderBorders(false, false);
			for (size_t i = 1; i < SnakeLength; i++) {
				POSCursor(SnakePOSbuffer[i][0], SnakePOSbuffer[i][1]);
				printf("#");
			}
			if (!BallEaten && !GenBall) {
				POSCursor(BallX, BallY);
				printf("O");
			}			
			PrintGameStats();
			doPause = false;
			return;
		} else if (pressed & WPAD_BUTTON_HOME) {
			exit(0);
		}
	}
}

void RunGame() {
	sleep(Speed);
	if (doPause) {
		Pause();
	}
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
	ManageSnakePos();
	RenderSnake();
	PressedButton = false;
	VIDEO_WaitVSync();

}

int main(int argc, char **argv) {
	srand(time(NULL));
	SystemInit();
	PlayOgg(sample_ogg, sample_ogg_size,  0, OGG_ONE_TIME);
	printf("\x1b[2J");
	printf("SnakeWii\nMade By Abdelali221\nGithub : https://github.com/abdelali221/\n");
	printf("\nPress A to start...");

	while(!Resume) {

		WPAD_ScanPads();
		u32 pressed = WPAD_ButtonsDown(0);
		if ( pressed & WPAD_BUTTON_A ) {
			Resume = true;
		}
		VIDEO_WaitVSync();
	}

	printf("\x1b[2J");
	DifficultySelect();
	RenderBorders(true, true);

	while (1) {
		RunGame();
	}
}