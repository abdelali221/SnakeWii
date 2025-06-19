#include "Video.h"
#include "WiiLibs.h"
#include "FreeMonoBold_ttf.h"
#include "ball_png.h"
#include "blank_png.h"
#include "snake_png.h"

GRRLIB_ttfFont *myFont;
GRRLIB_texImg *BlackScreen;
GRRLIB_texImg *CopiedImg;
GRRLIB_texImg *tex_ball;
GRRLIB_texImg *tex_snake;
GRRLIB_texImg *tex_blank;
u32 textX = 0;
u32 textY = 0;

void textprint(const char *text) {
    GRRLIB_PrintfTTF(textX, textY, myFont, text, 12, 0xFFFFFFFF);
}

void valprint(int val) {
    char valtext[5];
    snprintf(valtext, sizeof(valtext), "%d", val);
    GRRLIB_PrintfTTF(textX, textY, myFont, valtext, 12, 0xFFFFFFFF);
}

void DrawBall(uint X, uint Y) {
    GRRLIB_DrawImg(X*10, Y*10, tex_ball, 0, 1, 1, 0xFFFFFFFF);
}

void DrawSnake(uint X, uint Y) {
    GRRLIB_DrawImg(X*10, Y*10, tex_snake, 0, 1, 1, 0xFFFFFFFF);
}

void DrawBlank(uint X, uint Y) {
    GRRLIB_DrawImg(X*10, Y*10, tex_blank, 0, 1, 1, 0xFFFFFFFF);
}

void POSCursor(uint X, uint Y) {
    textX = X * 10;
    textY = Y * 10;
}

void ClearScreen() {
    GRRLIB_FillScreen(0x000000FF);  
    POSCursor(40, 27);
	textprint("No one is illegal."); 
	POSCursor(1, 1);
    RenderBuffer();
}

void VideoInit() {
	GRRLIB_Init();
    GRRLIB_Settings.antialias = false;
	myFont = GRRLIB_LoadTTF(FreeMonoBold_ttf, FreeMonoBold_ttf_size);
    BlackScreen = GRRLIB_CreateEmptyTexture(rmode->fbWidth, rmode->efbHeight);
    CopiedImg = GRRLIB_CreateEmptyTexture(rmode->fbWidth, rmode->efbHeight);
    tex_ball = GRRLIB_LoadTexture(ball_png);
    tex_blank = GRRLIB_LoadTexture(blank_png);
    tex_snake = GRRLIB_LoadTexture(snake_png);
    GRRLIB_SetBackgroundColour(0x00, 0x00, 0x00, 0xFF);
}

void GRR_Exit() {
    GRRLIB_FreeTexture(BlackScreen);
    GRRLIB_FreeTexture(CopiedImg);
    GRRLIB_FreeTexture(tex_ball);
    GRRLIB_FreeTexture(tex_blank);
    GRRLIB_FreeTexture(tex_snake);
    GRRLIB_FreeTTF(myFont);
    GRRLIB_Exit();
}

void RenderBuffer() {
    VIDEO_WaitVSync();
    GRRLIB_Render();
}