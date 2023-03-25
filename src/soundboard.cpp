#include <Arduino.h>
#include "soundboard.h"
#include "defines.h"

#define TRIGGER_PULSE_LENGHT 100

typedef struct {
    enum trigger_states state;
    uint32_t timer;
    uint8_t pin;
} sound_t;

sound_t sounds[8];

void init_sound(void){
    memset(sounds, 0, sizeof(sound_t) * 8);

    // sounds[0].pin = SOUND_GOOD_TP;
    // sounds[1].pin = SOUND_BAD_TP;
    // sounds[2].pin = SOUND_WIN_TP;
    // sounds[3].pin = SOUND_RES_1_TP;
    // sounds[4].pin = SOUND_RES_2_TP;
    // sounds[5].pin = SOUND_RES_3_TP;
    // sounds[6].pin = SOUND_RES_4_TP;
    // sounds[7].pin = SOUND_RES_5_TP;
}

void trigger_sound(enum sound_idx idx){
    if(idx < 8){
        sounds[idx].state = TRG_REQ; //Request trigger
    }
}

void tick_sound(void){
    for(uint8_t i = 0; i < 8; i++){
        switch(sounds[i].state){
            case TRG_OFF:
                digitalWrite(sounds[i].pin, HIGH);
                break;
            case TRG_REQ:
                digitalWrite(sounds[i].pin, LOW);
                sounds[i].state = TRG_ACT;
                break;
            case TRG_ACT:
                if(millis() - sounds[i].timer > TRIGGER_PULSE_LENGHT){
                    sounds[i].state = TRG_OFF;
                }
                break;
            default:
                break;
        }
    }
}