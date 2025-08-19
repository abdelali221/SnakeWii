#ifndef _UTILS_H_
#define _UTILS_H_
#include "WiiLibs.h"

void VideoInit();
void CheckController();
void sleep(u32 delay);
void PrintGameStats();
void RenderBorders(bool DELAY, bool PLAYSOUND);
void GenerateBall();
bool Loose();
void RunGame();

#endif