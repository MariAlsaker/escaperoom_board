#include <Arduino.h>
#include "gamestate.h"
#include "defines.h"
#include "lock_control.h"
#include "com.h"
#include "cmd.h"

enum gamestates gamestate = GS_A;

uint32_t timestamp = 0;
uint32_t sync_timestamp = 0;
uint8_t sync_payload[8];


void init_gamestate(void){
    timestamp = millis();
    sync_timestamp = millis();

    memset(sync_payload, 0, 8);
} 

// Airway Handler
static inline handler_returns handler_a(void){
    if(!digitalRead(AO_STATE_PIN)){
        return RET_DONE;
    }else{
        return RET_INPROGRESS;
    }
}

// Breathing Handler
static inline handler_returns handler_b(void){
    if(!digitalRead(PT_STATE_PIN_ERROR)){
        return RET_ERROR;
    }

    if(!digitalRead(PT_STATE_PIN)){
        return RET_DONE;
    }else{
        return RET_INPROGRESS;
    }
}

// Circulation Handler
static inline handler_returns handler_c(void){
    if(!digitalRead(BL_STATE_PIN)){
        return RET_DONE;
    }else{
        return RET_INPROGRESS;
    }
}

// Disability Handler
static inline handler_returns handler_d(void){
    if(!digitalRead(CO_STATE_PIN)){
        return RET_DONE;
    }else{
        return RET_INPROGRESS;
    }
}

// Exposure Handler
static inline handler_returns handler_e(void){
    if(!digitalRead(CO_STATE_PIN)){
        return RET_DONE;
    }else{
        return RET_INPROGRESS;
    }
}

void tick_gamestate(void){
    handler_returns ret;
    switch(gamestate){
        //Airway
        case GS_A:
            sync_payload[0] = IN_PROGRESS;
            //open_lock(LOCK_A);
            if(handler_a() == RET_DONE){
                sync_payload[0] = SOLVED;
                gamestate = GS_B;
            }
            break;
        //Breathing
        case GS_B:
            sync_payload[1] = IN_PROGRESS;
            ret = handler_b();
            if(ret == RET_DONE){
                sync_payload[1] = SOLVED;
                gamestate = GS_C;
            }
            break;
        //Breathing Timeout
        case GS_B_TIMEOUT:
            sync_payload[1] = TIMEOUT;
            if(millis() - timestamp > B_TIMEOUT_MS){
                gamestate = GS_B;
            }
            break;
        //Circulation
        case GS_C:
            sync_payload[2] = IN_PROGRESS;
            if(handler_c() == RET_DONE){
                sync_payload[2] = SOLVED;
                gamestate = GS_D;
            }
            break;
        //Disability
        case GS_D:
            sync_payload[3] = IN_PROGRESS;
            ret = handler_d();
            if(ret == RET_ERROR){
                timestamp = millis();
                gamestate = GS_D_TIMEOUT;
            }else if(ret == RET_DONE){
                sync_payload[3] = SOLVED;
                gamestate = GS_E;
            }
            break;
        //Disability Timeout
        case GS_D_TIMEOUT:
            sync_payload[3] = TIMEOUT;
            if(millis() - timestamp > D_TIMEOUT_MS){
                gamestate = GS_D;
            }
            break;
        //Exposure
        case GS_E:
            sync_payload[4] = IN_PROGRESS;
            ret = handler_e();
            if(ret == RET_ERROR){
                timestamp = millis();
                gamestate = GS_D_TIMEOUT;
            }else if(ret == RET_DONE){
                sync_payload[4] = SOLVED;
                gamestate = GS_FINISHED;
            }
            break;
        //Exposure Timeout
        case GS_E_TIMEOUT:
            sync_payload[4] = TIMEOUT;
            if(millis() - timestamp > E_TIMEOUT_MS){
                gamestate = GS_E;
            }
            break;
        //Finish
        case GS_FINISHED:
            break;
    }
}