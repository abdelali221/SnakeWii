#ifndef _UTILS_H_
#define _UTILS_H_
#include "WiiLibs.h"

void VideoInit();
void CheckController();
void sleep(u32 delay);
void PrintGameStats();
bool Loose();
void RunGame();

#endif