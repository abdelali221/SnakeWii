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

int BallX, BallY;
int SnakeX = COLS/2;
int SnakeY = ROWS/2;
int VSnakeX = 1;
int VSnakeY = 0;
int Lifes = 3;
int Score = 0;
int SnakeLength = 2;
int counter = 0;
int SnakePOSbuffer[255][2];

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
	} else if ((pressed & WPAD_BUTTON_UP) && (VSnakeX != 0 || !Start)) {
		if (!Start) {
			Start = true;
		}
		VSnakeY = -1;
		VSnakeX = 0;
	} else if ((pressed & WPAD_BUTTON_DOWN) && (VSnakeX != 0 || !Start)) {
		if (!Start) {
			Start = true;
		}
		VSnakeY = 1;
		VSnakeX = 0;
	} else if ((pressed & WPAD_BUTTON_LEFT) && (VSnakeY != 0 || !Start)) {
		if (!Start) {
			Start = true;
		}
		VSnakeX = -1;
		VSnakeY = 0;
	} else if ((pressed & WPAD_BUTTON_RIGHT) && (VSnakeY != 0 || !Start)) {
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
	for (int Y = VER_OFFSET; Y <= ROWS; Y++) {
  
		for (int X = HOR_OFFSET; X <= COLS; X++) {
  
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
	while (BallX < HOR_OFFSET + 1 || BallX > COLS || BallY < VER_OFFSET + 1 || BallY > ROWS) {
		BallX = HOR_OFFSET + 1 + rand() % (COLS - HOR_OFFSET - 1);
		BallY = VER_OFFSET + 1 + rand() % (ROWS - VER_OFFSET - 1);
	}

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
	for (size_t i = 2; i < SnakeLength; i++) {
		if (BallX == SnakePOSbuffer[i][0] && SnakePOSbuffer[i][1]) {
			POSCursor(BallX, BallY);
			printf("O");
		}
	}
	
}

static void Loose() {
	printf("\x1b[2J");
	sleep(1000);
	RenderBorders(false);
	GenBall = true;
	Start = false;
	for (size_t i = 0; i < 254; i++) {
		SnakePOSbuffer[i][0] = 0;
		SnakePOSbuffer[i][1] = 0;
	}
	
	if (Lifes > 0) {
		Lifes--;
		SnakeX = COLS/2;
		SnakeY = ROWS/2;
		VSnakeX = 0;
		VSnakeY = 0;
	} else {
		SnakeX = COLS/2;
		SnakeY = ROWS/2;
		VSnakeX = 0;
		VSnakeY = 0;
		Lifes = 3;
		Score = 0;
	}
}

static void ManageSnakePos() {
	CheckController();
	if (SnakeX < HOR_OFFSET || SnakeX > COLS || SnakeY < VER_OFFSET || SnakeY > ROWS) {
		Loose();
	}
	if ( SnakeLength > 4) {
		for (size_t i = 2; i < SnakeLength; i++) {
			if (SnakeX == SnakePOSbuffer[i][0] && SnakeY == SnakePOSbuffer[i][1]) {
				Loose();
			}
		}
	}
		
	if (SnakeX == BallX && SnakeY == BallY) {
		Score++;
		SnakeLength++;
		GenBall = true;
	}
	SnakeX = SnakeX + VSnakeX;
	SnakeY = SnakeY + VSnakeY;
}

static void PrintGameStats() {
	POSCursor(0, ROWS + 2);
	printf(" Score : %d ", Score);
}
static void RunGame() {
	sleep(500);
	PrintGameStats();
	ManageSnakePos();
	RenderSnake();
	if (counter < 4) {
		counter++;
	} else {
		counter = 0;
	}
	if (counter == 3 && GenBall) {
		GenerateBall();
		GenBall = false;
	}
	VIDEO_WaitVSync();
}

int main(int argc, char **argv) {
	srand(time(NULL));
	SystemInit();
	printf("\x1b[2J");
	printf("Snake Wii\nWritten By Abdelali221\nGithub : https://github.com/abdelali221/\n");
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
	RenderBorders(true);

	while (1) {
		RunGame();
	}
}