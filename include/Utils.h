#ifndef _UTILS_H_
#define _UTILS_H_
#include "WiiLibs.h"
#include "Variables.h"
#include "WiiVT.h"
#include "OGGFiles.h"

void CheckController();
void sleep(u32 delay);
void RenderBorders(bool DELAY, bool PLAYSOUND);
void GenerateBall();
void DifficultySelect();

#endif