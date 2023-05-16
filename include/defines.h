#ifndef DEFINES_H
#define DEFINES_H

#include <Arduino.h>

#define TESTMODE

#define LCD_RS              2
#define LCD_RW              3
#define LCD_EN              4
#define LCD_D4              5
#define LCD_D5              6
#define LCD_D6              7
#define LCD_D7              8

#define REDLED_PIN          9
#define GREENLED_PIN        10

#define AO_STATE_PIN        11 // Airway Obstruction
#define BL_STATE_PIN        12 // Bleeding
#define PT_STATE_PIN_ERROR  13 // Pneumothorax Error
#define PT_STATE_PIN        14 // Pneumothorax


#define KEYPAD_ROW1         15
#define KEYPAD_ROW2         16
#define KEYPAD_ROW3         17
#define KEYPAD_ROW4         18
#define KEYPAD_COL1         19
#define KEYPAD_COL2         20
#define KEYPAD_COL3         21

#endif