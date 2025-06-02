#include "Utils.h"

void CheckController() {
	WPAD_ScanPads();
	u32 pressed = WPAD_ButtonsDown(0);
	if (pressed & WPAD_BUTTON_HOME) {
		exit(0);
	} else if ((pressed & WPAD_BUTTON_UP) && (VSnakeX != 0 || !Start) && !PressedButton) {
		PressedButton = true;
		if (!Start) {
			Start = true;
		}
		VSnakeY = -1;
		VSnakeX = 0;
	} else if ((pressed & WPAD_BUTTON_DOWN) && (VSnakeX != 0 || !Start) && !PressedButton) {
		PressedButton = true;
		if (!Start) {
			Start = true;
		}
		VSnakeY = 1;
		VSnakeX = 0;
	} else if ((pressed & WPAD_BUTTON_LEFT) && (VSnakeY != 0 || !Start) && !PressedButton) {
		PressedButton = true;
		if (!Start) {
			Start = true;
		}
		VSnakeX = -1;
		VSnakeY = 0;
	} else if ((pressed & WPAD_BUTTON_RIGHT) && (VSnakeY != 0 || !Start) && !PressedButton) {
		PressedButton = true;
		if (!Start) {
			Start = true;
		}
		VSnakeX = 1;
		VSnakeY = 0;
	} else if (pressed & WPAD_BUTTON_PLUS) {
		doPause = true; 
	}
}

void sleep(u32 delay) {
	u32 ticks = delay/20;
	u32 start = 0;
	while (start < ticks) {
		CheckController();
		start++;
		VIDEO_WaitVSync();
	}
}

void RenderBorders(bool DELAY, bool PLAYSOUND) {
	for (size_t Y = VER_OFFSET; Y <= ROWS; Y++) {
  
		for (size_t X = HOR_OFFSET; X <= COLS; X++) {
  
			if ( ( (X == HOR_OFFSET || X == COLS) && (Y >= VER_OFFSET && Y <= ROWS) )|| (Y == VER_OFFSET || Y == ROWS)) {
  				POSCursor(X, Y);
				if (DELAY) {
  					sleep(10);
				}
		  		printf("#");
				VIDEO_WaitVSync();
  			}
		}
	}
	if (PLAYSOUND) {
		PlayOgg(start_ogg, start_ogg_size, 0, OGG_ONE_TIME);
	}	
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

void DifficultySelect() {
	uint8_t Selection = 10;
	POSCursor(4, 8);
	printf("Choose the difficulty :");
	POSCursor(10, 10);
	printf("Easy");
	POSCursor(10, 12);
	printf("Medium");
	POSCursor(10, 14);
	printf("Hard");
	POSCursor(9, Selection);
	printf(">");
	while(1) {
		WPAD_ScanPads();
		u32 pressed = WPAD_ButtonsDown(0);

		if ((pressed & WPAD_BUTTON_DOWN) && Selection < 14) {
			POSCursor(9, Selection);
			printf(" ");
			Selection = Selection + 2;
			POSCursor(9, Selection);
			printf(">");
			PlayOgg(select_ogg, select_ogg_size,  0, OGG_ONE_TIME);
		} else if ((pressed & WPAD_BUTTON_UP) && Selection > 10) {
			POSCursor(9, Selection);
			printf(" ");
			Selection = Selection - 2;
			POSCursor(9, Selection);
			printf(">");
			PlayOgg(select_ogg, select_ogg_size,  0, OGG_ONE_TIME);
		} else if (pressed & WPAD_BUTTON_A) {
			printf("\x1b[2J");
			 
			switch (Selection)
			{
				case 10:
					Speed = 500;
					PlayOgg(easy_ogg, easy_ogg_size,  0, OGG_ONE_TIME);
				break;
				
				case 12:
					Speed = 250;
					PlayOgg(medium_ogg, medium_ogg_size,  0, OGG_ONE_TIME);
				break;

				case 14:
					Speed = 100;
					PlayOgg(hard_ogg, hard_ogg_size,  0, OGG_ONE_TIME);
				break;

				default:
				break;
			}
			return;
		} else if (pressed & WPAD_BUTTON_HOME) {
			exit(0);
		}
	}
}