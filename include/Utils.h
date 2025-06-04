#ifndef _UTILS_H_
#define _UTILS_H_
#include "WiiLibs.h"
#include "Variables.h"
#include "WiiVT.h"

void SystemInit();
void CheckController();
void sleep(u32 delay);
void PrintGameStats();
void RenderBorders(bool DELAY, bool PLAYSOUND);
void GenerateBall();
void DifficultySelect();
void Pause();
void GameOver();
void Loose();

#endif