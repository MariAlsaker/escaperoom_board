#ifndef DEFINES_H
#define DEFINES_H

#include <Arduino.h>

#define TESTMODE

#define LCD_RW 43
#define LCD_RS 41
#define LCD_EN 39
#define LCD_D4 31
#define LCD_D5 33
#define LCD_D6 35
#define LCD_D7 37

#define LED1_PIN 11
#define LED2_PIN 12
#define LED3_PIN 13

#define BUZZER_PIN 8

#define LEDSTRIP_PIN 7

#define AO_STATE_PIN        14 // Airway Obstruction
#define PT_STATE_PIN        15 // Pneumothorax
#define PT_STATE_PIN_ERROR  16 // Pneumothorax Error
#define BL_STATE_PIN        17 // Bleeding
#define CO_STATE_PIN        18 // Consciousness
#define RES1_STATE_PIN      19
#define RES2_STATE_PIN      20
#define RES3_STATE_PIN      21

#define KEYPAD_ROW1 40
#define KEYPAD_ROW2 42
#define KEYPAD_ROW3 44
#define KEYPAD_ROW4 46
#define KEYPAD_COL1 48
#define KEYPAD_COL2 50
#define KEYPAD_COL3 52

#endif