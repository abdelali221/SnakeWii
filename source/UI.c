#include "UI.h"
#include "WiiLibs.h"
#include "Utils.h"
#include "SnakeUtils.h"
#include "Variables.h"
#include "Audio.h"
#include "Video.h"
#include "Input.h"
#include "SaveFile.h"

int8_t MenuSelect = 0;
const uint8_t konamicode[9] = {1, 2, 2, 3, 4, 3, 4, 5, 6};

void Pause() {
	ClearScreen();
	Play(PAUSE);
	while (1) {
		POSCursor(36, 10);
		textprint("Paused!");
		POSCursor(30, 12);
		textprint("+ : Resume");
		POSCursor(30, 14);
		textprint("- : End the game");
		POSCursor(30, 16);
		textprint("HOME : Main Menu");
		RenderBuffer();
		int pressed = CheckWPAD();

		if (pressed == PLUS) {
			ingame = true;
			Play(RESUME);
			ClearScreen();
			RenderBorders(false, false);
			for (size_t i = 1; i < SnakeLength; i++) {
				DrawSnake(SnakePOSbuffer[i][0], SnakePOSbuffer[i][1]);
			}
			if (!BallEaten && !GenBall) {
				DrawBall(BallX, BallY);
			}
			doPause = false;
			break;
		} else if (pressed == MINUS) {
			doPause = false;
			Lives = 0;
			Loose();
			return;
		} else if (pressed == HOME) {
			ingame = false;
			return;
		}
	}
}

void MainMenu() {
	while (1)
	{	
		int pressed = CheckWPAD();

		if (pressed == DOWN && MenuSelect < 8) {
			MenuSelect = MenuSelect + 2;
			Play(SELECT);
		} else if (pressed == UP && MenuSelect > 0) {
			MenuSelect = MenuSelect - 2;
			Play(SELECT);
		} else if ( (pressed == b_A) || (pressed == TWO) ) {
			ClearScreen();
			switch (MenuSelect)
			{
				case 0:
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
					return;
				break;

				default:
				break;
			}
		}
		POSCursor(3, 2);
		textprint("  /$$$$$$  /$$   /$$  /$$$$$$  /$$   /$$ /$$$$$$$$ /$$      /$$ /$$ /$$");
		POSCursor(3, 3);
		textprint(" /$$__  $$| $$$ | $$ /$$__  $$| $$  /$$/| $$_____/| $$  /$ | $$|__/|__/");
		POSCursor(3, 4);
		textprint("| $$   __/| $$$$| $$| $$    $$| $$ /$$/ | $$      | $$ /$$$| $$ /$$ /$$");
		POSCursor(3, 5);
		textprint("|  $$$$$$ | $$ $$ $$| $$$$$$$$| $$$$$/  | $$$$$   | $$/$$ $$ $$| $$| $$"); 
		POSCursor(3, 6);
		textprint("  ____  $$| $$  $$$$| $$__/ $$| $$  $$  | $$__/   | $$$$_  $$$$| $$| $$");
		POSCursor(3, 7);
		textprint(" /$$    $$| $$   $$$| $$  | $$| $$   $$ | $$      | $$$/    $$$| $$| $$ ");
		POSCursor(3, 8);
		textprint("|  $$$$$$/| $$    $$| $$  | $$| $$    $$| $$$$$$$$| $$/      $$| $$| $$");
		POSCursor(3, 9);
		textprint("  ______/ |__/   __/|__/  |__/|__/   __/|________/|__/      __/|__/|__/ ");
		POSCursor(50, 10);
		textprint("Ver 2.0");
		POSCursor(28, 12);
		if (doPause) {
			textprint("Resume Game");
		} else {
			textprint("New Game");
		}
		POSCursor(28, 14);
		textprint("Settings");
		POSCursor(28, 16);
		textprint("Game Speed");
		POSCursor(28, 18);
		textprint("Credits");
		POSCursor(28, 20);
		textprint("Exit");
		POSCursor(25, 12 + MenuSelect);
		textprint(">");
		POSCursor(40, 15);
		textprint("High Score : ");
		POSCursor(50, 15);
		valprint(HighScore);
		POSCursor(40, 27);
		textprint("No one is illegal."); 
		RenderBuffer();
	}
}

void DifficultySelect() {
	uint8_t Selection = 0;
	while (1)
	{		
		POSCursor(4, 8);
		textprint("Choose the difficulty :");
		POSCursor(10, 10);
		textprint("Easy");
		POSCursor(10, 12);
		textprint("Medium");
		POSCursor(10, 14);
		textprint("Hard");
		POSCursor(8, 10 + Selection);
		textprint(">");
		POSCursor(40, 27);
		textprint("No one is illegal."); 
		int pressed = CheckWPAD();

		if (pressed == DOWN && Selection < 4) {
			Selection = Selection + 2;
			Play(SELECT);
		} else if (pressed == UP && Selection > 0) {
			Selection = Selection - 2;
			Play(SELECT);
		} else if (pressed == b_A || pressed == TWO) {
			ClearScreen();
			switch (Selection)
			{
				case 0:
					Speed = 250;
					Play(EASY);
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
			return;
		} else if (pressed == b_B || pressed == ONE) {
			ClearScreen();
			return;
		}
		RenderBuffer();
	}
}

void GameOver() {
	Lives = 3;
	while (1)
	{
		POSCursor(34, 10);
		textprint("Game Over!");
		if (Score > HighScore) {
			POSCursor(28, 12);
			textprint("New High Score :       !");
			POSCursor(38, 12);
			valprint(HighScore);
		} else {
			POSCursor(32, 12);
			textprint("Your Score : ");
			POSCursor(42, 12);
			valprint(Score);
		}
		Score = 0;
		POSCursor(21, 14);
		textprint("Press A (or 2) to go back to the menu.");
		POSCursor(40, 27);
		textprint("No one is illegal."); 
		RenderBuffer();
		int pressed = CheckWPAD();
		if (pressed == b_A || pressed == TWO) {
			if (Score > HighScore) {
				HighScore = Score;
				SaveHighScore();
			}
			return;
		}
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
	textprint("Credits : Andy Sloane (AIKON), donut.c author.");
	POSCursor(17, 26);
	textprint("Music by Jogeir Liljedahl.");
	POSCursor(40, 27);
	textprint("No one is illegal."); 
	Play(DONUT);
	while (1) {
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
        textprint("\x1b[H");
        for(k = 0; k < 1761; k++) {
            putchar(k % 80 ? b[k] : 10);
            A += 0.00004;
            B += 0.00002;
        }
        sleep(30);
		int pressed = CheckWPAD();
		if (pressed == b_B || pressed == ONE) {
			StopPlaying();
			ClearScreen();
			break;
		}
    }
}

void CreditsMenu() {
	while (true) {
		POSCursor(4, 2);
		textprint("SnakeWii, Developed by Abdelali221.");
		POSCursor(5, 6);
		textprint("This game is entirely open source, if you want to port it");
		POSCursor(2, 8);
		textprint("just make sure to give credits to the original coder (Abdelali221).");
		POSCursor(4, 12);
		textprint("Special thanks to :");
		POSCursor(4, 14);
		textprint("- Posifurg : Sound files.");
		POSCursor(8, 22);
		textprint("Github : https://github.com/abdelali221/SnakeWii/");
		POSCursor(8, 24);
		textprint("Press B (1) to go back...");
		POSCursor(40, 27);
		textprint("No one is illegal."); 
		RenderBuffer();
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
	uint8_t Selection = 0;
	while (1) {
		POSCursor(20, 5);
		textprint("Settings :");
		POSCursor(25, 10);
		textprint("WiiMote : ");
		POSCursor(35, 10);
		if (WPADType) {
			textprint("Vertical  ");
		} else {
			textprint("Horizontal");
		}
		POSCursor(25, 12);
		textprint("SFX : ");
		POSCursor(31, 12);
		if (SFX) {
			textprint("ON");
		} else {
			textprint("OFF");
		}
		POSCursor(23, 10 + Selection);
		textprint(">");
		POSCursor(40, 27);
		textprint("No one is illegal.");
		RenderBuffer();
		int pressed = CheckWPAD();
		if (pressed == DOWN && Selection < 2) {
			Selection = Selection + 2;
			Play(SELECT);
		} else if (pressed == UP && Selection > 0) {
			Selection = Selection - 2;
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
		} else if (pressed == b_B || pressed == ONE) {
			SaveSettings();
			return;
		}
	}
}