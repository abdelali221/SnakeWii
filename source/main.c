#include "Video.h"
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
	MainMenu();
	GRR_Exit();
	return 0;
}