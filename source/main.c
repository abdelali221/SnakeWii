#include "SnakeUtils.h"
#include "WiiVT.h"
#include "WiiLibs.h"
#include "Variables.h"
#include "Utils.h"
#include "Audio.h"

void RunGame() {
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
	sleep(Speed/2);
	ManageSnakePos();
	RenderSnake();
	sleep(Speed/2);
	PressedButton = false;
	VIDEO_WaitVSync();
}

int main(int argc, char **argv) {
	srand(time(NULL));
	WPAD_Init();
	VideoInit();
	MainMenu();
	printf("\x1b[2J");
	RenderBorders(true, true);

	while (1) {
		RunGame();
	}
}