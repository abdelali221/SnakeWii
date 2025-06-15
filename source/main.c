#include "WiiVT.h"
#include "WiiLibs.h"
#include "Utils.h"
#include "UI.h"
#include "Audio.h"
#include "SaveFile.h"

int main(int argc, char **argv) {
	srand(time(NULL));
	WPAD_Init();
	ASND_Init();
	VideoInit();
	fatInitDefault();
	DetectFATDevice();
	CheckForSav();
	LoadFromSav();
	ClearScreen();
	Volume(255);
	
	while (1) {
		MainMenu();
	}
}