#include <Arduino.h>
#include "lock_control.h"
#include "defines.h"

#define HIT_DURATION 100

typedef struct {
    enum hithold_states state;
    uint32_t timer;
    uint8_t pin;
} lock_t;

lock_t locks[5];

void init_lockcontrol(void){
    memset(locks, 0, sizeof(lock_t) * 5);
    
    // locks[0].pin = LOCK_A_PIN;
    // locks[1].pin = LOCK_B_PIN;
    // locks[2].pin = LOCK_C_PIN;
    // locks[3].pin = LOCK_D_PIN;
    // locks[4].pin = LOCK_E_PIN;
}

void open_lock(enum lock_index idx){
    Serial.print("Opening lock ");
    Serial.println(idx);
    if(idx < 5){
        locks[idx].state = HH_REQ; //Request opening
    }
}

void close_lock(enum lock_index idx){
    Serial.print("Closing lock ");
    Serial.println(idx);
    if(idx < 5){
        locks[idx].state = HH_OFF; //Close
    }
}

void reset_locks(void){
    for(uint8_t i = 0; i < 5; i++){
        locks[i].state = HH_OFF;
    }
}

void tick_locks(void){
    for(uint8_t i = 0; i < 5; i++){
        switch(locks[i].state){
            case HH_OFF:
                analogWrite(locks[i].pin, 0);
                break;
            case HH_REQ:
                locks[i].timer = millis();
                locks[i].state = HH_HIT;
                break;
            case HH_HIT:
                analogWrite(locks[i].pin, 255);
                if(millis() - locks[i].timer > HIT_DURATION){
                    locks[i].state = HH_HOLD;
                }
                break;
            case HH_HOLD:
                analogWrite(locks[i].pin, 128);
                break;
            default:
                break;
        }
    }

    #ifdef TESTMODE
        static int8_t lock_idx = 0;
        static uint32_t timer = 0;
        static uint8_t opening = 1;

        if(millis() - timer > 1000){
            timer = millis();
            if(opening){
                open_lock((enum lock_index) lock_idx++);
                if(lock_idx > LOCK_E){
                    opening = 0;
                }
            }else{
                close_lock((enum lock_index) --lock_idx);
                if(lock_idx == LOCK_A){
                    opening = 1;
                }
            }
        }

    #endif
}