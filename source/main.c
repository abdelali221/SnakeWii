#include "SnakeUtils.h"
#include "WiiVT.h"
#include "WiiLibs.h"
#include "Variables.h"
#include "Utils.h"
#include "Audio.h"

#define VER "1.3"

void RunGame() {
	if (doPause)
		Pause();
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
	sleep(Speed);
	ManageSnakePos();
	RenderSnake();
	PressedButton = false;
	VIDEO_WaitVSync();

}

int main(int argc, char **argv) {
	srand(time(NULL));
	SystemInit();
	printf("\x1b[2J");
	POSCursor(10, 5);
	printf(" #####  #     #    #    #    # ####### #     # ### ###");
	POSCursor(10, 6);
	printf("#     # ##    #   # #   #   #  #       #  #  #  #   #");
	POSCursor(10, 7);
	printf("#       # #   #  #   #  #  #   #       #  #  #  #   #");
	POSCursor(10, 8);
	printf(" #####  #  #  # #     # ###    #####   #  #  #  #   #");
	POSCursor(10, 9);
	printf("      # #   # # ####### #  #   #       #  #  #  #   #"); 
	POSCursor(10, 10);
	printf("#     # #    ## #     # #   #  #       #  #  #  #   #");
	POSCursor(10, 11);
	printf(" #####  #     # #     # #    # #######  ## ##  ### ###   Ver %s", VER);
	POSCursor(15, 14);
	printf("Made By Abdelali221");
	POSCursor(10, 16);
	printf("Github : https://github.com/abdelali221/");
	POSCursor(20, 18);
	printf("Press A to start...");
	Play(SAMPLE);
	while(!Resume) {

		WPAD_ScanPads();
		u32 pressed = WPAD_ButtonsDown(0);
		if ( pressed & WPAD_BUTTON_A ) {
			Resume = true;
		} else if ( pressed & WPAD_BUTTON_HOME ) {
			exit(0);
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