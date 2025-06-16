#include "SaveFile.h"
#include "Variables.h"
#include "WiiVT.h"

char* PATHHISCORE;
char* PATHSETTINGS;
FILE* savfile;

void LoadFromSav() {
    HighScore = 0;
    savfile = fopen(PATHHISCORE, "r");
    HighScore = getw(savfile);
    fclose(savfile);

    char data[2];
    savfile = fopen(PATHSETTINGS, "r");
    fread(data, sizeof(data), 2, savfile);
    if (data[0] == '0') {
        WPADType = false;
    } else {
        WPADType = true;
    }

    if (data[1] == '0') {
        SFX = false;
    } else {
        SFX = true;
    }
    fclose(savfile);
}

void CheckForSav() {
    savfile = fopen(PATHHISCORE, "r");
    if (savfile == NULL) {
        savfile = fopen(PATHHISCORE, "w+");
        putw(0, savfile);
    }
    fclose(savfile);

    savfile = fopen(PATHSETTINGS, "r");
    if (savfile == NULL) {
        char write[2] = {(WPADType + 48), (SFX + 48)};
        savfile = fopen(PATHSETTINGS, "w+");
        for (size_t i = 0; i < sizeof(write); i++)
        {
            fprintf(savfile, "%c", write[i]);
        }        
    }
    fclose(savfile);
}

void SaveHighScore() {
    savfile = fopen(PATHHISCORE, "w");
    putw(HighScore, savfile);
    fclose(savfile);
}

void SaveSettings() {
    char write[2] = {(WPADType + 48), (SFX + 48)};
    savfile = fopen(PATHSETTINGS, "w");
    for (size_t i = 0; i < sizeof(write); i++)
    {
        fprintf(savfile, "%c", write[i]);
    }        
    fclose(savfile);
}

void DetectFATDevice() {
    FILE* test = fopen("sd:/apps/SnakeWii/TEST", "w+");
    if (test == NULL) {
        test = fopen("usb:/apps/SnakeWii/TEST", "w+");
        if (test == NULL) {
            fclose(test);
            exit(0);
        } else {
            PATHSETTINGS = "usb:/apps/SnakeWii/Settings.sav";
            PATHHISCORE = "usb:/apps/SnakeWii/HighScore.sav";
            fclose(test);
            remove("usb:/apps/SnakeWii/TEST");
        }
    } else {
        PATHSETTINGS = "sd:/apps/SnakeWii/Settings.sav";
        PATHHISCORE = "sd:/apps/SnakeWii/HighScore.sav";
        fclose(test);
        remove("sd:/apps/SnakeWii/TEST");
    }
}