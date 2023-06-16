#include <Keypad.h>
#include "input.h"
#include "defines.h"
#include "gamestate.h"

const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
char keys[ROWS][COLS] = {
{'1','2','3'},
{'4','5','6'},
{'7','8','9'},
{'*','0','#'}
};

byte row_pins[ROWS] = {KEYPAD_ROW1, KEYPAD_ROW2, KEYPAD_ROW3, KEYPAD_ROW4}; //connect to the row pinouts of the kpd
byte col_pins[COLS] = {KEYPAD_COL1, KEYPAD_COL2, KEYPAD_COL3}; //connect to the column pinouts of the kpd

Keypad kpd = Keypad(makeKeymap(keys), row_pins, col_pins, ROWS, COLS);

String debug_msg;

uint8_t input_enabled = 0;

char input[8];
uint8_t input_idx = 0;

void init_input(void){

}

uint8_t debug_input(void){
    if(kpd.getKeys()){
        for (int i = 0; i < LIST_MAX; i++){ // Scan the whole key list.
            if (kpd.key[i].stateChanged){
                        if(kpd.key[i].kstate == PRESSED && kpd.key[i].kchar == '#'){
                            return 1;
                        }else{
                            return 0;
                        }
            }
        }
    }
}

void tick_input(void){
    // Fills kpd.key[ ] array with up-to 10 active keys.
    // Returns true if there are ANY active keys.
    if (kpd.getKeys())
    {
        for (int i = 0; i < LIST_MAX; i++){ // Scan the whole key list.

            if (kpd.key[i].stateChanged){   // Only find keys that have changed state.
                switch(kpd.key[i].kstate){  // Report active key state : IDLE, PRESSED, HOLD, or RELEASED
                    case PRESSED:
                        //debug_msg = " PRESSED.";
                        if(kpd.key[i].kchar == '*'){
                            wipe_char_gamestate();
                        }else if(kpd.key[i].kchar == '#'){
                            request_verification();
                        }else{
                            input_char_gamestate(kpd.key[i].kchar);
                        }
                        break;
                    case RELEASED:
                        break;
                    default:
                        break;
                }
                // Serial.print("Key ");
                // Serial.print(kpd.key[i].kchar);
                // Serial.println(debug_msg);
            }
        }
    }
}