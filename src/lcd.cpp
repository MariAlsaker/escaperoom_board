#include <Arduino.h>
#include <LiquidCrystal.h>
#include <iostream>
#include "lcd.h"
#include "defines.h"

static LiquidCrystal* lcd;

static uint32_t lcd_dirty = 1;
static lcd_states lcd_state = L_IDLE;
static uint32_t last_update;
static uint32_t blink_timestamp = 0;
static uint8_t blink_state = 1;

char textbuffer[11] = {'>', ' ', ' ',' ',' ',' ',' ',' ',' ','\0'};

#define LCD_UPDATE_INTERVAL 100

void init_lcd(void)
{
    lcd = new LiquidCrystal(LCD_RS, LCD_RW, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);
    lcd->begin(16, 2);
    last_update = millis();
    blink_timestamp = millis();
    lcd_state = L_IDLE;
}

void set_lcd_dirty(void){ lcd_dirty = 1; }

char* get_numberbufstart(void){ return &textbuffer[2]; }

void set_state_lcd(lcd_states newstate){ 
    lcd_state = newstate; 
    lcd_dirty = 1;
}

void debug_state_lcd(uint8_t* states, uint8_t len){
    if(len == 4){
        char* ptr = get_numberbufstart();
        for(int i = 0; i < 4; i++){
            if(states[i]){
                *ptr = '1';
            }else{
                *ptr = '0';
            }
            ptr++;
        }
        set_lcd_dirty();

    }
}

void tick_lcd(void)
{
    if(millis() - last_update > LCD_UPDATE_INTERVAL){ //Slow LCD refresh rate
        last_update = millis();

        if(lcd_dirty){
            lcd->clear();
        }

        switch(lcd_state)
        {
            case L_IDLE:
               if(millis() - blink_timestamp > 500){
                    blink_timestamp = millis();
                    blink_state = !blink_state;
                }
                if(blink_state){
                    lcd->setCursor(0, 0);
                    lcd->print("ERR DEVICE");
                    lcd->setCursor(0, 1);
                    lcd->print("MALFUNCTION");
                }else{
                    lcd->clear();
                }
                break;
            case L_INPUT_D:
                if(lcd_dirty){
                    lcd->setCursor(0, 0);
                    lcd->print("0x0D INPUT:");
                    lcd->setCursor(0, 1);
                    lcd->print(textbuffer);
                }
                break;
            case L_INPUT_E:
                if(lcd_dirty){
                    lcd->setCursor(0, 0);
                    lcd->print("0x0E INPUT:");
                    lcd->setCursor(0, 1);
                    lcd->print(textbuffer);
                }
                break;
            case L_ERROR:
               if(millis() - blink_timestamp > 500){
                    blink_timestamp = millis();
                    blink_state = !blink_state;
                    lcd_dirty = 1;
                }
                if(lcd_dirty){
                    lcd->setCursor(0, 0);
                    lcd->print("ERR 0x1824:");
                    lcd->setCursor(0, 1);
                    if(blink_state){
                        lcd->print("WRONG INPUT!");
                    }else{
                        lcd->print("            ");
                    }
                }
                break;
            case L_SUCCESS:
                if(lcd_dirty){
                    lcd->setCursor(0, 0);
                    lcd->print("INPUT ACCEPTED");
                    lcd->setCursor(0, 1);
                    lcd->print("...");
                }
                break;
            case L_FINISH:
                if(lcd_dirty){
                    lcd->setCursor(0, 0);
                    lcd->print("SYSTEM ONLINE");
                    lcd->setCursor(0, 1);
                    lcd->print("STATUS 0xF721");
                }
                break;
            case L_DEBUG:
                if(lcd_dirty){
                    lcd->setCursor(0, 0);
                    lcd->print("DEBUG MODE");
                    lcd->setCursor(0, 1);
                    lcd->print(textbuffer);
                }
                break;
        }
        lcd_dirty = 0;
    }
}