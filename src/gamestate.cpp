#include <Arduino.h>
#include "gamestate.h"
#include "defines.h"
#include "cmd.h"
#include "lcd.h"
#include "led_control.h"
#include "input.h"

uint32_t timestamp_debug = 0;
char debug_text[128];

enum gamestates gamestate = GS_A;
uint32_t timestamp = 0;
uint32_t sync_timestamp = 0;
uint8_t sync_payload[8];

char* numberbuf;
uint8_t input_accepted;

uint8_t inputver_requested = 0;

void init_gamestate(void){
    timestamp = millis();
    numberbuf = get_numberbufstart();
    input_accepted = 0;
}

void poll_debug_gamestate(void){
    if(debug_input()){
        gamestate = GS_DEBUG;
    }
}

uint8_t input_char_gamestate(char input)
{
    if(!input_accepted){ return 0; }

    if(numberbuf != get_numberbufstart() + 8){
        *numberbuf = input;
        numberbuf++;
        *numberbuf = '\0'; //Overflowing is expected
        return 1;
    }else{
        return 0;
    }
}

void wipe_char_gamestate(void)
{
    if(!input_accepted){ return; }

    numberbuf = get_numberbufstart();
    memset(numberbuf, ' ', sizeof(char) * 8);
}

uint8_t request_verification(void)
{
    if(!input_accepted){ return 0; }

    inputver_requested = 1;
    return 1;
}

// Airway Handler
static inline handler_returns handler_a(void){
    set_state_lcd(L_IDLE);
    if(!digitalRead(AO_STATE_PIN)){
        return RET_DONE;
    }else{
        return RET_INPROGRESS;
    }
}

// Breathing Handler
static inline handler_returns handler_b(void){
    set_state_lcd(L_IDLE);
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
    set_state_lcd(L_IDLE);
    if(!digitalRead(BL_STATE_PIN)){
        return RET_DONE;
    }else{
        return RET_INPROGRESS;
    }
}

// Disability Handler
static inline handler_returns handler_d(void){
    set_state_lcd(L_INPUT_D);
    input_accepted = 1;
    if(inputver_requested){
        inputver_requested = 0;
        char* input = get_numberbufstart();
        Serial.println(input);
        if(!strncmp(input, "333", sizeof(char) * 8)){
            wipe_char_gamestate();
            return RET_DONE;
        }else{
            wipe_char_gamestate();
            return RET_ERROR;
        }
    }
    return RET_INPROGRESS;

}

// Exposure Handler
static inline handler_returns handler_e(void){
    set_state_lcd(L_INPUT_E);
    input_accepted = 1;
    if(inputver_requested){
        inputver_requested = 0;
        char* input = get_numberbufstart();
        Serial.println(input);
        if(!strncmp(input, "111", sizeof(char) * 8)){
            wipe_char_gamestate();
            return RET_DONE;
        }else{
            wipe_char_gamestate();
            return RET_ERROR;
        }
    }
    return RET_INPROGRESS;
}

static inline handler_returns handler_debug(void){
    uint8_t io_states[4];
    io_states[0] = digitalRead(AO_STATE_PIN);
    io_states[1] = digitalRead(BL_STATE_PIN);
    io_states[2] = digitalRead(PT_STATE_PIN_ERROR);
    io_states[3] = digitalRead(PT_STATE_PIN);
    debug_state_lcd(io_states, 4);
}

void tick_gamestate(void){

    if(millis() - timestamp_debug > 500){
        timestamp_debug = millis();
        snprintf(debug_text, 128 * sizeof(char), "(GS: %d) [%d, %d, %d, %d]",
            gamestate,
            !digitalRead(AO_STATE_PIN), !digitalRead(PT_STATE_PIN), 
            !digitalRead(PT_STATE_PIN_ERROR), !digitalRead(BL_STATE_PIN));
        Serial.println(debug_text);
    }

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
            set_ledmode(LED_G, LED_OFF);
            set_ledmode(LED_R, LED_OFF);
            if(ret == RET_ERROR){
                timestamp = millis();
                gamestate = GS_D_TIMEOUT;
            }else if(ret == RET_DONE){
                sync_payload[3] = SOLVED;
                timestamp = millis();
                gamestate = GS_D_SUCCESS;
            }
            break;
        //Disability success time
        case GS_D_SUCCESS:
            set_state_lcd(L_SUCCESS);
            set_ledmode(LED_G, LED_ON);
            set_ledmode(LED_R, LED_OFF);
            if(millis() - timestamp > SUCCESS_TIME){
                timestamp = millis();
                gamestate = GS_E;
            }
            break;
        //Disability Timeout
        case GS_D_TIMEOUT:
            set_state_lcd(L_ERROR);
            set_ledmode(LED_G, LED_OFF);
            set_ledmode(LED_R, LED_PULSE);
            sync_payload[3] = TIMEOUT;
            if(millis() - timestamp > D_TIMEOUT_MS){
                gamestate = GS_D;
            }
            break;
        //Exposure
        case GS_E:
            sync_payload[4] = IN_PROGRESS;
            ret = handler_e();
            set_ledmode(LED_G, LED_OFF);
            set_ledmode(LED_R, LED_OFF);
            if(ret == RET_ERROR){
                timestamp = millis();
                gamestate = GS_E_TIMEOUT;
            }else if(ret == RET_DONE){
                sync_payload[4] = SOLVED;
                timestamp = millis();
                gamestate = GS_E_SUCCESS;
            }
            break;
        //Exposure success time
        case GS_E_SUCCESS:
            set_state_lcd(L_SUCCESS);
            set_ledmode(LED_G, LED_ON);
            set_ledmode(LED_R, LED_OFF);
            if(millis() - timestamp > SUCCESS_TIME){
                timestamp = millis();
                gamestate = GS_FINISHED;
            }
            break;
        //Exposure Timeout
        case GS_E_TIMEOUT:
            set_state_lcd(L_ERROR);
            set_ledmode(LED_G, LED_OFF);
            set_ledmode(LED_R, LED_PULSE);
            sync_payload[4] = TIMEOUT;
            if(millis() - timestamp > E_TIMEOUT_MS){
                gamestate = GS_E;
            }
            break;
        //Finish
        case GS_FINISHED:
            set_state_lcd(L_FINISH);
            set_ledmode(LED_G, LED_PULSE);
            set_ledmode(LED_R, LED_OFF);
            break;
        //Debug
        case GS_DEBUG:
            handler_debug();
            set_state_lcd(L_DEBUG);
            set_ledmode(LED_G, LED_PULSE);
            set_ledmode(LED_R, LED_PULSE);
            break;
    }
}