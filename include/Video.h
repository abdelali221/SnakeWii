#ifndef _VIDEO_H_
#define _VIDEO_H_
#include "WiiLibs.h"
#include <grrlib.h>

extern GRRLIB_texImg *CopiedImg;

void textprint(const char *text);
void valprint(int val);
void DrawSnake(uint X, uint Y);
void DrawBall(uint X, uint Y);
void DrawBlank(uint X, uint Y);
void POSCursor(uint X, uint Y);
void ClearScreen();
void VideoInit();
void GRR_Exit();
void RenderBuffer();

#endif