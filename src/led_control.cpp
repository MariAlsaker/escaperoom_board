#include <Arduino.h>
#include "led_control.h"
#include "defines.h"

#define RESOLUTION 10

static led_t leds[LED_COUNT];

void init_ledcontrol(void){
    memset(leds, 0, sizeof(led_t) * LED_COUNT);
    
    leds[0].pin = REDLED_PIN;
    leds[1].pin = GREENLED_PIN;

    #ifdef TESTMODE
        leds[0].mode = LED_OFF;
        leds[0].intensity = 255;
        leds[0].speed = 5;
        
        leds[1].mode = LED_OFF;
        leds[1].intensity = 255;
        leds[1].speed = 5;
    #endif
}

void set_ledmode(enum led_index led, enum led_mode mode){
    leds[led].mode = mode;
}

uint8_t pin_software_pwm(uint8_t pin){
    // #ifdef ARDUINO_AVR_UNO
    //     switch(pin){
    //         case 3:
    //         case 5:
    //         case 6:
    //         case 9:
    //         case 10:
    //         case 11:
    //             return 0;
    //         default:
    //             return 1;
    //     }
    // #else
    //     #error "Unsupported board"
    // #endif
}

void tick_leds(void){
    for(uint8_t i = 0; i < LED_COUNT; i++){
        switch(leds[i].mode){
            case LED_OFF:
                leds[i].power = 0;
                break;
            case LED_BLINK:
                if(millis() - leds[i].timer > leds[i].speed){
                    leds[i].timer = millis();
                    if(!leds[i].power){
                        leds[i].power = leds[i].intensity;
                    }else{
                        leds[i].power = 0;
                    }
                }
                break;
            case LED_PULSE:
                if(millis() - leds[i].timer > RESOLUTION){
                    leds[i].timer = millis();

                    if(leds[i].increasing){
                        leds[i].power += leds[i].speed; //Increment power
                        
                        if(leds[i].power >= 255){ //Saturate to 255
                            leds[i].power = 255;
                        }

                        if(leds[i].power >= leds[i].intensity){ //When intensity is reached
                           leds[i].increasing = 0; //Start decrement
                        }
                    }else{
                        if((leds[i].power - leds[i].speed) < 0){ //Protect against underflow
                            leds[i].power = 0;
                        }else{
                            leds[i].power -= leds[i].speed;
                        }

                        if(leds[i].power == 0){ //When 0 is met
                           leds[i].increasing = 1; //Start incrementing again
                        }
                    }
                }
                break;
            case LED_FADEOUT:
                if(millis() - leds[i].timer > RESOLUTION){
                    leds[i].timer = millis();
                    if((leds[i].power - leds[i].speed) < 0){ //Protect against underflow
                        leds[i].power = 0;
                    }else{
                        leds[i].power -= leds[i].speed;
                    }

                    if(leds[i].power == 0){ //When 0 is met
                        leds[i].mode = LED_OFF; //Mode is off
                    }
                }
                break;
            case LED_FADEIN:
                if(millis() - leds[i].timer > RESOLUTION){
                    leds[i].timer = millis();
                    leds[i].power += leds[i].speed; //Increment power
                    
                    if(leds[i].power >= 255){ //Saturate to 255
                        leds[i].power = 255;
                    }

                    if(leds[i].power >= leds[i].intensity){ //When intensity is met
                        leds[i].mode = LED_ON; //Mode is on
                    }
                }
                break;
            case LED_ON:
                leds[i].power = leds[i].intensity;
                break;
            default:
                leds[i].power = 0;
                break;
        }

        // if(pin_software_pwm(leds[i].pin)){
        //     if(millis() - leds[i].software_pwm_timer > 10){
        //         leds[i].software_pwm_timer = millis();
        //         if(map(leds[i].intensity))
        //         digitalWrite(leds[i].pin, digitalRead(leds[i].pin));
        //     }
        // }else{
            analogWrite(leds[i].pin, 255 - leds[i].power); //Apply power
        //}
    }
}