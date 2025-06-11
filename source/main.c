#include "WiiVT.h"
#include "WiiLibs.h"
#include "Utils.h"
#include "UI.h"

int main(int argc, char **argv) {
	srand(time(NULL));
	WPAD_Init();
	VideoInit();
	ClearScreen();
	
	while (1) {
		MainMenu();
	}
}