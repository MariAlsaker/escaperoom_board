#include <Arduino.h>
#include <LiquidCrystal.h>
#include "lcd.h"
#include "defines.h"

static LiquidCrystal* lcd;

static lcd_states lcd_state = L_IDLE;
static uint32_t last_update;
static uint32_t transition_timer;

#define LCD_UPDATE_INTERVAL 100

void init_lcd(void)
{
    lcd = new LiquidCrystal(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);
    lcd->begin(16, 2);
    last_update = millis();
}

void tick_lcd(void)
{
    if(millis() - last_update > LCD_UPDATE_INTERVAL){ //Slow LCD refresh rate
        last_update = millis();
        switch(lcd_state)
        {
            case L_IDLE:
                lcd->clear();
                lcd->setCursor(0, 0);
                lcd->print("ERR: 0x3d21");
                lcd->setCursor(0, 1);
                lcd->print("...");
                break;
            case L_INPUT:
                lcd->clear();
                lcd->setCursor(0, 0);
                lcd->print("Ready for input:");
                lcd->setCursor(0, 1);
                lcd->print("x");
                break;
            case L_ERROR:
                lcd->clear();
                lcd->setCursor(0, 0);
                lcd->print("ERR: WRONG INPUT");
                break;
            case L_SUCCESS:
                lcd->clear();
                lcd->setCursor(0, 0);
                lcd->print("INPUT ACCEPTED!");
                if(millis() - transition_timer > 3000){
                    lcd_state = L_INPUT;
                }
            
        }
    }
}