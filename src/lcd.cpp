#include <Arduino.h>
#include <LiquidCrystal.h>
#include "lcd.h"
#include "defines.h"

static LiquidCrystal* lcd;

static lcd_states lcd_state = L_STARTUP;
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
            case L_STARTUP:
                lcd_state = L_INFO;
                break;
            case L_INFO:
                lcd->clear();
                lcd->setCursor(0, 0);
                lcd->print("ME&T");
                lcd->setCursor(0, 1);
                lcd->print("Sneezeman");
                break;
            case L_INFO_SNEEZE:
                lcd->clear();
                lcd->setCursor(0, 0);
                lcd->print("Sneezing...");
                break;
            case L_INFO_SNEEZE_THANKS:
                lcd->clear();
                lcd->setCursor(0, 0);
                lcd->print("Enjoy sneeze!");
                if(millis() - transition_timer > 1000){
                    lcd_state = L_INFO;
                }
                break;
        }
    }
}