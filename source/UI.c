#include "UI.h"
#include "WiiLibs.h"
#include "Utils.h"
#include "Variables.h"
#include "Audio.h"
#include "WiiVT.h"
#include "Input.h"

uint8_t MenuSelect = 0;

void Pause() {
	while (1)
	{	
		ClearScreen();
		POSCursor(34, 10);
		printf("Paused!");
		POSCursor(27, 12);
		printf("Press + to resume or");
		POSCursor(18, 14);
		printf(" press HOME to go back to the Main Menu");
		Play(PAUSE);
		while (1) {
			int pressed = CheckWPAD();

			if (pressed == PLUS) {
				ingame = true;
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
			} else if (pressed == HOME) {
				ingame = false;
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
		printf("  ____  $$| $$  $$$$| $$__/ $$| $$  $$  | $$__/   | $$$$_  $$$$| $$| $$");
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
		printf("Settings");
		POSCursor(28, 16);
		printf("Game Speed");
		POSCursor(28, 18);
		printf("Credits");
		POSCursor(28, 20);
		printf("Exit");
		POSCursor(25, 12 + MenuSelect);
		printf(">");
		while(1) {
			int pressed = CheckWPAD();

			if (pressed == DOWN && MenuSelect < 8) {
				POSCursor(25, 12 + MenuSelect);
				printf(" ");
				MenuSelect = MenuSelect + 2;
				POSCursor(25, 12 + MenuSelect);
				printf(">");
				Play(SELECT);
			} else if (pressed == UP && MenuSelect > 0) {
				POSCursor(25, 12 + MenuSelect);
				printf(" ");
				MenuSelect = MenuSelect - 2;
				POSCursor(25, 12 + MenuSelect);
				printf(">");
				Play(SELECT);
			} else if ( (pressed == b_A) || (pressed == TWO) ) {
				ClearScreen();
				switch (MenuSelect)
				{
					case 0:
						ClearScreen();
						RunGame();
					break;

					case 2:
						Settings();
					break;
					
					case 4:
						DifficultySelect();
					break;

					case 6:
						CreditsMenu();
					break;

					case 8:
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
		int pressed = CheckWPAD();

		if (pressed == DOWN && Selection < 14) {
			POSCursor(9, Selection);
			printf(" ");
			Selection = Selection + 2;
			POSCursor(9, Selection);
			printf(">");
			Play(SELECT);
		} else if (pressed == UP && Selection > 10) {
			POSCursor(9, Selection);
			printf(" ");
			Selection = Selection - 2;
			POSCursor(9, Selection);
			printf(">");
			Play(SELECT);
		} else if (pressed == b_A || pressed == TWO) {
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
		}
	}
}

void GameOver() {
	POSCursor(34, 10);
	printf("Game Over!");
	POSCursor(32, 12);
	printf("Your Score : %d", Score);
	POSCursor(24, 14);
	printf("Press A (or 2) to go back to the menu.");
	while (true) {
		int pressed = CheckWPAD();
		if (pressed == b_A || pressed == TWO) {
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
		ingame = false;
		Play(LOST);
		sleep(2000);
		ClearScreen();
		RenderBorders(false, true);
		Lives--;
		ingame = true;
		return false;
	} else {
		ingame = false;
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
	printf("Credits : Andy Sloane (AIKON), Original donut code author.");
	POSCursor(17, 26);
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
                int y= 10 + 15 * D * (l * h * n + t * m);
                int o = x + 80 * y;
                int N = 8 * ((f * e - c * d * g) * m - c * d * e - f * g - l * d * n);
                if(20 > y && y > 0 && x > 0 && 60 > x && D > z[o]) {
                    z[o] = D;
                    b[o] = ".,-~:;=!*#$@"[N > 0 ? N : 0];
                }
            }
        }
        printf("\x1b[H");
        for(k = 0; k < 1500; k++) {
            putchar(k % 80 ? b[k] : 10);
            A += 0.00004;
            B += 0.00002;
        }
        sleep(30);
		int pressed = CheckWPAD();
		if (pressed == b_B || pressed == ONE) {
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
	POSCursor(4, 12);
	printf("Special thanks to :");
	POSCursor(4, 14);
	printf("- Posifurg : Sound files.");
	POSCursor(8, 22);
	printf("Github : https://github.com/abdelali221/SnakeWii/");
	POSCursor(8, 24);
	printf("Press B (1) to go back...");
	while (true) {
		int pressed = CheckWPAD();
		if (pressed == b_B || pressed == ONE) {
			ClearScreen();
			return;
		} else if (pressed == UP) {
			uint8_t secretcode[9];
			for (size_t i = 0; i < 9; i++)
			{
				while (1)
				{
					int pressed = CheckWPAD();
					if (pressed != 0) {
						if (pressed < 7) {
							if (i > 6) {
								secretcode[i] = pressed;
							} else if (pressed < 5 && i < 7) {
								secretcode[i] = pressed;
							}
							break;
						} else {
							return;
						}
					}
				}				
			}
			for (size_t i = 0; i < 9; i++) {
				if (secretcode[i] != konamicode[i]) {
					return;			
				}
			}
			donut();
			return;
		}
	}
}

void Settings() {
	ClearScreen();
	uint8_t Selection = 0;
	POSCursor(20, 5);
	printf("Settings :");
	POSCursor(25, 10);
	printf("WiiMote : ");
	if (WPADType) {
		printf("Vertical  ");
	} else {
		printf("Horizontal");
	}
	POSCursor(23, 10 + Selection);
	printf(">");
	while(1) {
		int pressed = CheckWPAD();

		if (pressed == DOWN && Selection < 0) {
			POSCursor(23, 10 + Selection);
			printf(" ");
			Selection = Selection + 2;
			POSCursor(23, 10 + Selection);
			printf(">");
			Play(SELECT);
		} else if (pressed == UP && Selection > 0) {
			POSCursor(23, 10 + Selection);
			printf(" ");
			Selection = Selection - 2;
			POSCursor(23, 10 + Selection);
			printf(">");
			Play(SELECT);
		} else if (pressed == b_A || pressed == TWO) {
		 
			switch (Selection)
			{
				case 0:
					POSCursor(35, 10);
					if (WPADType) {
						WPADType = false;
						printf("Horizontal");
					} else {
						WPADType = true;
						printf("Vertical  ");
					}
				break;
				
				case 2:
					Speed = 125;
					Play(MEDIUM);
				break;

				case 4:
					Speed = 80;
					Play(HARD);
				break;

				default:
				break;
			}
		} else if (pressed == b_B || pressed == ONE) {
			return;
		}
	}
}