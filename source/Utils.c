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
		ClearScreen();
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
				ClearScreen();
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
	{	ClearScreen();
		POSCursor(3, 2);
		printf("  /$$$$$$  /$$   /$$  /$$$$$$  /$$   /$$ /$$$$$$$$ /$$      /$$ /$$ /$$");
		POSCursor(3, 3);
		printf(" /$$__  $$| $$$ | $$ /$$__  $$| $$  /$$/| $$_____/| $$  /$ | $$|__/|__/");
		POSCursor(3, 4);
		printf("| $$   __/| $$$$| $$| $$    $$| $$ /$$/ | $$      | $$ /$$$| $$ /$$ /$$");
		POSCursor(3, 5);
		printf("|  $$$$$$ | $$ $$ $$| $$$$$$$$| $$$$$/  | $$$$$   | $$/$$ $$ $$| $$| $$"); 
		POSCursor(3, 6);
		printf("  ____  $$| $$  $$$$| $$__  $$| $$  $$  | $$__/   | $$$$_  $$$$| $$| $$");
		POSCursor(3, 7);
		printf(" /$$    $$| $$   $$$| $$  | $$| $$   $$ | $$      | $$$/    $$$| $$| $$ ");
		POSCursor(3, 8);
		printf("|  $$$$$$/| $$    $$| $$  | $$| $$    $$| $$$$$$$$| $$/      $$| $$| $$");
		POSCursor(3, 9);
		printf("  ______/ |__/   __/|__/  |__/|__/   __/|________/|__/      __/|__/|__/ ");
		POSCursor(68, 10);
		printf("Ver %s", VER);
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
				ClearScreen();
				switch (MenuSelect)
				{
					case 0:
						ClearScreen();
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
			ClearScreen();
			 
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
					Speed = 80;
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
		ClearScreen();
		RenderBorders(false, true);
		Lives--;
		return false;
	} else {
		Play(DIED);
		sleep(4000);
		ClearScreen();
		GameOver();
		Lives = 3;
		Score = 0;
		return true;
	}
}

void donut() {
    float A = 0, B = 0;
    float i, j;
    int k;
    float z[1760];
    char b[1760];
    ClearScreen();
	POSCursor(8, 24);
	printf("Credits : ANdy Sloane (AIKON), Original donut code author.");
	POSCursor(16, 26);
	printf("Akhilesh Thile, donut.c author.");
    for(;;) {
		memset(b,32,1760);
        memset(z,0,7040);
        for(j=0; j < 6.28; j += 0.07) {
            for(i=0; i < 6.28; i += 0.02) {
                float c = sin(i);
                float d = cos(j);
                float e = sin(A);
                float f = sin(j);
                float g = cos(A);
                float h = d + 2;
                float D = 1 / (c * h * e + f * g + 5);
                float l = cos(i);
                float m = cos(B);
                float n = sin(B);
                float t = c * h * g - f * e;
                int x = 40 + 30 * D * (l * h * m - t * n);
                int y= 12 + 15 * D * (l * h * n + t * m);
                int o = x + 80 * y;
                int N = 8 * ((f * e - c * d * g) * m - c * d * e - f * g - l * d * n);
                if(22 > y && y > 0 && x > 0 && 80 > x && D > z[o]) {
                    z[o] = D;
                    b[o] = ".,-~:;=!*#$@"[N > 0 ? N : 0];
                }
            }
        }
        printf("\x1b[H");
        for(k = 0; k < 1761; k++) {
            putchar(k % 80 ? b[k] : 10);
            A += 0.00004;
            B += 0.00002;
        }
        sleep(30);
		WPAD_ScanPads();
		u32 pressed = WPAD_ButtonsDown(0);
		if (pressed & WPAD_BUTTON_B) {
			ClearScreen();
			break;
		}
    }
}

void CreditsMenu() {
	ClearScreen();
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
			ClearScreen();
			return;
		} else if (pressed & WPAD_BUTTON_UP) {
			while (1)
			{
				WPAD_ScanPads();
				u32 pressed = WPAD_ButtonsDown(0);
				if (pressed & WPAD_BUTTON_UP) {
					while (1)
					{
						WPAD_ScanPads();
						u32 pressed = WPAD_ButtonsDown(0);
						if (pressed & WPAD_BUTTON_DOWN) {
							while (1)
							{
								WPAD_ScanPads();
								u32 pressed = WPAD_ButtonsDown(0);
								if (pressed & WPAD_BUTTON_DOWN) {
									while (1)
									{
										WPAD_ScanPads();
										u32 pressed = WPAD_ButtonsDown(0);
										if (pressed & WPAD_BUTTON_LEFT) {
											while (1)
											{
												WPAD_ScanPads();
												u32 pressed = WPAD_ButtonsDown(0);
												if (pressed & WPAD_BUTTON_RIGHT) {
													while (1)
													{
														WPAD_ScanPads();
														u32 pressed = WPAD_ButtonsDown(0);
														if (pressed & WPAD_BUTTON_LEFT) {
															while (1)
															{
																WPAD_ScanPads();
																u32 pressed = WPAD_ButtonsDown(0);
																if (pressed & WPAD_BUTTON_RIGHT) {
																	while (1)
																	{
																		donut();
																		return;
																	}
																}
															}
														}
													}											
												}
											}											
										}
									}											
								}
							}							
						}
					}					
				}
			}			
		}
	}
}