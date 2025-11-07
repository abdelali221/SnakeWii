#include "WiiVT.h"

void POSCursor(uint8_t X, uint8_t Y) {
	printf("\x1b[%d;%dH", Y, X);
}

void ClearScreen() {
	printf("\x1b[2J");
}
