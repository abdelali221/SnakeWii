#include "SnakeUtils.h"
#include "Video.h"
#include "WiiLibs.h"
#include "Variables.h"
#include "Audio.h"
#include "Utils.h"
#include "UI.h"

void RenderBorders(bool DELAY, bool PLAYSOUND) {
	ingame = false;
	for (size_t Y = VER_OFFSET; Y <= ROWS; Y++) {
  
		for (size_t X = HOR_OFFSET; X <= COLS; X++) {
  
			if ( ( (X == HOR_OFFSET || X == COLS) && (Y >= VER_OFFSET && Y <= ROWS) )|| (Y == VER_OFFSET || Y == ROWS)) {
  				POSCursor(X, Y);
				if (DELAY) {
  					sleep(10);
					RenderBuffer();
				}
		  		textprint("#");				
  			}
		}
	}
	if (PLAYSOUND) {
		Play(START);
	}
	ingame = true;
}

void RenderSnake() {
	for (size_t i = 0; i < SnakeLength; i++)
	{
		if (SnakePOSbuffer[i][0] != 0 && SnakePOSbuffer[i][1] != 0)
			DrawSnake(SnakePOSbuffer[i][0], SnakePOSbuffer[i][1]);
	}
	SnakePOSbuffer[0][0] = SnakeX;
	SnakePOSbuffer[0][1] = SnakeY;
	for (size_t i = SnakeLength; i > 0; i--) {
		SnakePOSbuffer[i][0] = SnakePOSbuffer[i - 1][0];
		SnakePOSbuffer[i][1] = SnakePOSbuffer[i - 1][1];
	}

}

void ManageSnakePos() {
	if (SnakeX < HOR_OFFSET + 1 || SnakeX > COLS - 1 || SnakeY < VER_OFFSET + 1 || SnakeY > ROWS - 1) {
		ClearScreen();
		if (Loose()) {
			ingame = false;
			doPause = true;
			return;
		}
	}
	if (SnakeLength > 4) {
		for (size_t i = 4; i < SnakeLength + 1; i++) {
			if (SnakeX == SnakePOSbuffer[i][0] && SnakeY == SnakePOSbuffer[i][1]) {
				ClearScreen();
				if (Loose()) {
					ingame = false;
					doPause = true;
					return;
				}
			}
		}
	}
		
	if (SnakeX == BallX && SnakeY == BallY && !BallEaten) {
		 
		Play(INCREASE);
		Score++;
		SnakeLength++;
		BallEaten = true;
		GenBall = true;
	}
	SnakeX = SnakeX + VSnakeX;
	SnakeY = SnakeY + VSnakeY;
}

void GenerateBall() {
	for (size_t i = 1; i <= SnakeLength; i++) {
		while (BallX < HOR_OFFSET + 1 || BallX > COLS || BallY < VER_OFFSET + 1 || BallY > ROWS || (BallX == SnakePOSbuffer[i][0] && BallY == SnakePOSbuffer[i][1]) || (BallX == ANSBallX && BallY == ANSBallY)) {
			BallX = HOR_OFFSET + 1 + rand() % (COLS - HOR_OFFSET - 1);
			BallY = VER_OFFSET + 1 + rand() % (ROWS - VER_OFFSET - 1);
		}
	}
	BallEaten = false;
	ANSBallX = BallX;
	ANSBallY = BallY;
}