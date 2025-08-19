#include "UI.h"
#include "WiiLibs.h"
#include "Utils.h"
#include "Variables.h"
#include "Audio.h"
#include "WiiVT.h"
#include "Input.h"
#include "SaveFile.h"

uint8_t MenuSelect = 0;
const uint8_t konamicode[9] = {1, 2, 2, 3, 4, 3, 4, 5, 6};

int Pause() {
	ClearScreen();
	POSCursor(36, 10);
	printf("Paused!");
	POSCursor(30, 12);
	printf("+ : Resume");
	POSCursor(30, 14);
	printf("- : End the game");
	POSCursor(30, 16);
	printf("HOME : Main Menu");
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
			break;
		} else if (pressed == MINUS) {
			return -1;
		} else if (pressed == HOME) {
			return -2;
		}
	}
	return 0;
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
        POSCursor(55, 15);
        if (HighScore == 255) {
            printf("High Score: >255 (big)");
        } else {
            printf("High Score: %d", HighScore);
        }
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
				Play(SUBSELECT);
				ClearScreen();
				switch (MenuSelect)
				{
					case 0:
					    if (Speed == 250) {
					        Play(EASY);
					    } else if (Speed == 125) {
					        Play(MEDIUM);
					    } else if (Speed == 80) {
					        Play(HARD);
					    }
						RunGame();
					break;

					case 2:
						Settings();
					break;
					
					case 4:
						if (!doPause)
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
	uint8_t Selection = 0;
	POSCursor(4, 8);
	printf("Choose the difficulty :");
	POSCursor(10, 10);
	printf("Easy");
	POSCursor(10, 12);
	printf("Medium");
	POSCursor(10, 14);
	printf("Hard");
	POSCursor(8, 10 + Selection);
	printf(">");
	while (1) {
		int pressed = CheckWPAD();

		if (pressed == DOWN && Selection < 4) {
			POSCursor(8, 10 + Selection);
			printf(" ");
			Selection = Selection + 2;
			POSCursor(8, 10 + Selection);
			printf(">");
			Play(SELECT);
		} else if (pressed == UP && Selection > 0) {
			POSCursor(8, 10 + Selection);
			printf(" ");
			Selection = Selection - 2;
			POSCursor(8, 10 + Selection);
			printf(">");
			Play(SELECT);
		} else if (pressed == b_A || pressed == TWO) {
			ClearScreen();
			 
			switch (Selection)
			{
				case 0:
					Speed = 250;
				break;
				
				case 2:
					Speed = 125;
				break;

				case 4:
					Speed = 80;
				break;

				default:
				break;
			}
			Play(SUBSELECT);
			return;
		} else if (pressed == b_B || pressed == ONE) {
			Play(BACK);
			ClearScreen();
			break;
		}
	}
}

void GameOver() {
	Lives = 3;
	doPause = false;
	POSCursor(34, 10);
	printf("Game Over!");
	
	if (Score > HighScore) {
		POSCursor(28, 12);
		printf("New High Score : %d !", Score);
		HighScore = Score;
		SaveHighScore();
	} else {
		POSCursor(32, 12);
		printf("Your Score : %d", Score);
	}
	Score = 0;
	POSCursor(21, 14);
	printf("Press A (or 2) to go back to the menu.");
	while (true) {
		int pressed = CheckWPAD();
		if (pressed == b_A || pressed == TWO) {
			return;
		}
	}
}

void CreditsMenu() {
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
			Play(BACK);
			ClearScreen();
			return;
		}
	}
}

void Settings() {
	uint8_t Selection = 0;
	while (1) {
		POSCursor(20, 5);
		printf("Settings :");
		POSCursor(25, 10);
		printf("WiiMote : ");
		if (WPADType) {
			printf("Vertical  ");
		} else {
			printf("Horizontal");
		}
		POSCursor(25, 12);
		printf("SFX : ");
		if (SFX) {
			printf("ON ");
		} else {
			printf("OFF");
		}
		POSCursor(23, 10 + Selection);
		printf(">");
		while(1) {
			int pressed = CheckWPAD();

			if (pressed == DOWN && Selection < 2) {
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
						if (WPADType) {
							WPADType = false;
						} else {
							WPADType = true;
						}
					break;
					
					case 2:
						if (SFX) {
							SFX = false;
						} else {
							SFX = true;
						}
					break;

					default:
					break;
				}
				Play(SUBSELECT);
				break;
			} else if (pressed == b_B || pressed == ONE) {
				Play(BACK);
				SaveSettings();
				return;;
			}
		}
	}
}