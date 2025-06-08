#include "Utils.h"
#include "Audio.h"

uint8_t MenuSelect = 0;

void VideoInit() {
	static void *xfb = NULL;
	static GXRModeObj *Vmode = NULL;

	VIDEO_Init();

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
	WPAD_ScanPads();
	u32 pressed = WPAD_ButtonsDown(0);
	if ((pressed & WPAD_BUTTON_UP) && (VSnakeX != 0 || !Start) && !PressedButton) {
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
		Pause();
	}
}

void RenderBorders(bool DELAY, bool PLAYSOUND) {
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

void Pause() {
	while (1)
	{	
		printf("\x1b[2J");
		POSCursor(34, 10);
		printf("Paused!");
		POSCursor(28, 12);
		printf("Press + to resume...");
		Play(PAUSE);
		while (1) {
			WPAD_ScanPads();
			u32 pressed = WPAD_ButtonsDown(0);
			if (pressed & WPAD_BUTTON_PLUS) {
				Play(RESUME);
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
				doPause = false;
				return;
			} else if (pressed & WPAD_BUTTON_HOME) {
				break;
			}
		}
		MainMenu();
	}
}

void MainMenu() {
	while (1)
	{	printf("\x1b[2J");
		POSCursor(8, 2);
		printf(" #####  #     #    #    #    # ####### #     # ### ###");
		POSCursor(8, 3);
		printf("#     # ##    #   # #   #   #  #       #  #  #  #   #");
		POSCursor(8, 4);
		printf("#       # #   #  #   #  #  #   #       #  #  #  #   #");
		POSCursor(8, 5);
		printf(" #####  #  #  # #     # ###    #####   #  #  #  #   #");
		POSCursor(8, 6);
		printf("      # #   # # ####### #  #   #       #  #  #  #   #"); 
		POSCursor(8, 7);
		printf("#     # #    ## #     # #   #  #       #  #  #  #   #");
		POSCursor(8, 8);
		printf(" #####  #     # #     # #    # #######  ## ##  ### ###   Ver %s", VER);
		POSCursor(28, 12);
		if (doPause) {
			printf("Resume Game");
		} else {
			printf("New Game");
		}
		POSCursor(28, 14);
		printf("Game Speed");
		POSCursor(28, 16);
		printf("Credits");
		POSCursor(28, 18);
		printf("Exit");
		POSCursor(25, 12 + MenuSelect);
		printf(">");
		while(1) {
			WPAD_ScanPads();
			u32 pressed = WPAD_ButtonsDown(0);

			if ((pressed & WPAD_BUTTON_DOWN) && MenuSelect < 6) {
				POSCursor(25, 12 + MenuSelect);
				printf(" ");
				MenuSelect = MenuSelect + 2;
				POSCursor(25, 12 + MenuSelect);
				printf(">");
				Play(SELECT);
			} else if ((pressed & WPAD_BUTTON_UP) && MenuSelect > 0) {
				POSCursor(25, 12 + MenuSelect);
				printf(" ");
				MenuSelect = MenuSelect - 2;
				POSCursor(25, 12 + MenuSelect);
				printf(">");
				Play(SELECT);
			} else if (pressed & WPAD_BUTTON_A) {
				printf("\x1b[2J");
				switch (MenuSelect)
				{
					case 0:
						return;
					break;
					
					case 2:
						DifficultySelect();
					break;

					case 4:
						CreditsMenu();
					break;

					case 6:
						exit(0);
					break;

					default:
					break;
				}
				break;
			}
		}
	}
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
			Play(SELECT);
		} else if ((pressed & WPAD_BUTTON_UP) && Selection > 10) {
			POSCursor(9, Selection);
			printf(" ");
			Selection = Selection - 2;
			POSCursor(9, Selection);
			printf(">");
			Play(SELECT);
		} else if (pressed & WPAD_BUTTON_A) {
			printf("\x1b[2J");
			 
			switch (Selection)
			{
				case 10:
					Speed = 250;
					Play(EASY);
				break;
				
				case 12:
					Speed = 125;
					Play(MEDIUM);
				break;

				case 14:
					Speed = 63;
					Play(HARD);
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

void GameOver() {
	POSCursor(34, 10);
	printf("Game Over!");
	POSCursor(32, 12);
	printf("Your Score : %d", Score);
	POSCursor(24, 14);
	printf("Press A to go back to the menu.");
	while (true) {
		WPAD_ScanPads();
		u32 pressed = WPAD_ButtonsDown(0);
		if (pressed & WPAD_BUTTON_A) {
			return;
		}
	}

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
	

	if (Lives > 0) {
		Play(LOST);
		sleep(2000);
		printf("\x1b[2J");
		RenderBorders(false, true);
		Lives--;
		return false;
	} else {
		Play(DIED);
		sleep(4000);
		printf("\x1b[2J");
		GameOver();
		Lives = 3;
		Score = 0;
		return true;
	}
}

void CreditsMenu() {
	printf("\x1b[2J");
	POSCursor(4, 2);
	printf("SnakeWii, Developed by Abdelali221.");
	POSCursor(5, 6);
	printf("This game is entirely open source, if you want to port it");
	POSCursor(2, 8);
	printf("just make sure to give credits to the original coder (Abdelali221).");
	POSCursor(8, 22);
	printf("Github : https://github.com/abdelali221/SnakeWii/");
	POSCursor(8, 24);
	printf("Press B to go back...");
	while (true) {
		WPAD_ScanPads();
		u32 pressed = WPAD_ButtonsDown(0);
		if (pressed & WPAD_BUTTON_B) {
			printf("\x1b[2J");
			return;
		}
	}
}