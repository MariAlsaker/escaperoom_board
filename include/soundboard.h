#ifndef SOUNDBOARD_H
#define SOUNDBOARD_H

enum sound_idx {
    SOUND_GOOD = 0,
    SOUND_BAD = 1,
    SOUND_WIN = 2,
    SOUND_RES_1 = 3,
    SOUND_RES_2 = 4,
    SOUND_RES_3 = 5,
    SOUND_RES_4 = 6
};

enum trigger_states {
    TRG_OFF,
    TRG_REQ,
    TRG_ACT
};

void init_sound(void);
void trigger_sound(enum sound_idx idx);
void tick_sound(void);

#endif