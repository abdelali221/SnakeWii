#include "Audio.h"
#include "WiiVT.h"
#include "oggplayer.h"
#include "Utils.h"

void Play(uint8_t ID) {
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
            sleep(300);
        break;
        
        default:
        break;
    }
}