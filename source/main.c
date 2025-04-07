#include <stdio.h>
#include <stdlib.h>
#include <gccore.h>
#include <wiiuse/wpad.h>
#include <time.h>

const uint8_t HOR_OFFSET = 10;
const uint8_t VER_OFFSET = 0;
const uint8_t COLS = (50 + HOR_OFFSET);
const uint8_t ROWS = (20 + VER_OFFSET);

static void *xfb = NULL;
static GXRModeObj *Vmode = NULL;

bool Resume = false;
bool GenBall = true;
bool Start = false;
bool PressedButton = false;
bool BallEaten = false;

int BallX, BallY, ANSBallX, ANSBallY;
int SnakeX = (COLS/2) + 4;
int SnakeY = ROWS/2;
int VSnakeX = 1;
int VSnakeY = 0;
int Lifes = 3;
int Score = 0;
int SnakeLength = 2;
int counter = 0;
int Speed;
int SnakePOSbuffer[6000][2];

static void SystemInit() {

	VIDEO_Init();

	WPAD_Init();

	Vmode = VIDEO_GetPreferredMode(NULL);

	xfb = MEM_K0_TO_K1(SYS_AllocateFramebuffer(Vmode));

	console_init(xfb, 20, 20, Vmode->fbWidth, Vmode->xfbHeight, Vmode->fbWidth * VI_DISPLAY_PIX_SZ);

	VIDEO_Configure(Vmode);

	VIDEO_SetNextFramebuffer(xfb);

	VIDEO_SetBlack(false);

	VIDEO_Flush();

	VIDEO_WaitVSync();
	if(Vmode->viTVMode&VI_NON_INTERLACE) VIDEO_WaitVSync();
	
}

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
	}
}

void sleep(float delay) {
	float ticks = delay/20;
	float start = 0;
	while (start < ticks) {
		CheckController();
		start++;
		VIDEO_WaitVSync();
	}
}

void POSCursor(uint8_t X, uint8_t Y) {
	printf("\x1b[%d;%dH", Y, X);
}

static void RenderBorders(bool DELAY) {
	for (size_t Y = VER_OFFSET; Y <= ROWS; Y++) {
  
		for (size_t X = HOR_OFFSET; X <= COLS; X++) {
  
			if ( ( (X == HOR_OFFSET || X == COLS) && (Y >= VER_OFFSET && Y <= ROWS) )|| (Y == VER_OFFSET || Y == ROWS)) {
  				POSCursor(X, Y);
				if (DELAY) {
  					sleep(10);
				}
		  		printf("#");
  			}
		}
	}
}

static void GenerateBall() {
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

static void RenderSnake() {
	POSCursor(SnakePOSbuffer[SnakeLength][0], SnakePOSbuffer[SnakeLength][1]);
	printf(" ");
	POSCursor(SnakeX, SnakeY);
	printf("#");	
	SnakePOSbuffer[0][0] = SnakeX;
	SnakePOSbuffer[0][1] = SnakeY;
	for (size_t i = SnakeLength; i > 0; i--) {
		SnakePOSbuffer[i][0] = SnakePOSbuffer[i - 1][0];
		SnakePOSbuffer[i][1] = SnakePOSbuffer[i - 1][1];
	}

}

static void DifficultySelect() {
	int Selection = 10;
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
		} else if ((pressed & WPAD_BUTTON_UP) && Selection > 10) {
			POSCursor(9, Selection);
			printf(" ");
			Selection = Selection - 2;
			POSCursor(9, Selection);
			printf(">");
		} else if (pressed & WPAD_BUTTON_A) {
			printf("\x1b[2J");
			switch (Selection)
			{
				case 10:
					Speed = 500;
				break;
				
				case 12:
					Speed = 250;
				break;

				case 14:
					Speed = 100;
				break;

				default:
				break;
			}
			return;
		}
	}
}

static void GameOver() {
	printf("\x1b[2J");
	POSCursor(30, 10);
	printf("Game Over!");
	POSCursor(27, 12);
	printf("Your Score : %d", Score);
	POSCursor(14, 14);
	printf("Press HOME to exit or A to restart the game");

	while (1) {
		WPAD_ScanPads();
		u32 pressed = WPAD_ButtonsDown(0);
		if (pressed & WPAD_BUTTON_HOME) {
			exit(0);
		} else if (pressed & WPAD_BUTTON_A) {
			printf("\x1b[2J");
			DifficultySelect();
			RenderBorders(true);
			return;
		}
	}

}

static void Loose() {
	printf("\x1b[2J");
	sleep(1000);
	GenBall = true;
	Start = false;
	for (size_t i = 1; i < 254; i++) {
		SnakePOSbuffer[i][0] = 0;
		SnakePOSbuffer[i][1] = 0;
	}
	SnakeX = (COLS/2) + 4;
	SnakeY = ROWS/2;
	VSnakeX = 0;
	VSnakeY = 0;
	SnakeLength = 2;
	if (Lifes > 0) {
		RenderBorders(false);
		Lifes--;
	} else {
		GameOver();
		Lifes = 3;
		Score = 0;
	}
}

static void ManageSnakePos() {
	CheckController();
	if (SnakeX < HOR_OFFSET + 1 || SnakeX > COLS - 1 || SnakeY < VER_OFFSET + 1 || SnakeY > ROWS - 1) {
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
		Score++;
		SnakeLength++;
		GenBall = true;
	}
	SnakeX = SnakeX + VSnakeX;
	SnakeY = SnakeY + VSnakeY;
}

static void PrintGameStats() {
	POSCursor(0, ROWS + 2);
	printf(" Score : %d \n", Score);
	printf(" Lifes : %d ", Lifes);
}
static void RunGame() {
	sleep(Speed);
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
	RenderBorders(true);

	while (1) {
		RunGame();
	}
}