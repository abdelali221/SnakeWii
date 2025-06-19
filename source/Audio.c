#include "Audio.h"
#include "Video.h"
#include "oggplayer.h"
#include "Utils.h"
#include "Variables.h"

#include "sample_ogg.h" // 0
#include "died_ogg.h" // 1
#include "increase_ogg.h" // 2
#include "lost_ogg.h" // 3
#include "start_ogg.h" // 4
#include "easy_ogg.h" // 6
#include "medium_ogg.h" // 7
#include "hard_ogg.h" // 8
#include "pause_ogg.h" // 9
#include "resume_ogg.h" // 10
#include "select_ogg.h" // 11
#include "donut_ogg.h" // 12

void Play(uint8_t ID) {
    if (SFX) {
        switch (ID)
        {       
            case 0:
                PlayOgg(sample_ogg, sample_ogg_size, 0, OGG_ONE_TIME);
            break;
            
            case 1:
                PlayOgg(died_ogg, died_ogg_size, 0, OGG_ONE_TIME);
            break;
            
            case 2:
                PlayOgg(increase_ogg, increase_ogg_size, 0, OGG_ONE_TIME);
            break;

            case 3:
                PlayOgg(lost_ogg, lost_ogg_size, 0, OGG_ONE_TIME);
            break;

            case 4:
                PlayOgg(start_ogg, start_ogg_size, 0, OGG_ONE_TIME);
            break;

            case 6:
                PlayOgg(easy_ogg, easy_ogg_size, 0, OGG_ONE_TIME);
            break;

            case 7:
                PlayOgg(medium_ogg, medium_ogg_size, 0, OGG_ONE_TIME);
            break;

            case 8:
                PlayOgg(hard_ogg, hard_ogg_size, 0, OGG_ONE_TIME);
            break;

            case 9:
                PlayOgg(pause_ogg, pause_ogg_size, 0, OGG_ONE_TIME);
            break;

            case 10:
                PlayOgg(resume_ogg, resume_ogg_size, 0, OGG_ONE_TIME);
            break;

            case 11:
                PlayOgg(select_ogg, select_ogg_size, 0, OGG_ONE_TIME);
                sleep(200);
            break;

            case 12:
                PlayOgg(donut_ogg, donut_ogg_size, 0, OGG_INFINITE_TIME);
            break;
            
            default:
            break;
        }
    }
}

void StopPlaying() {
    StopOgg();
}

void Volume(int volume) {
    SetVolumeOgg(volume);
}