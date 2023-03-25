#include <Arduino.h>
#include "defines.h"
#include "gamestate.h"
#include "lock_control.h"
#include "input.h"
#include "soundboard.h"
#include "lcd.h"
#include "led_control.h"
#include "com.h"
#include "ble.h"

uint32_t subroutine_timestamp = 0;

void setup() {

  Serial.begin(9600);

  // pinMode(LOCK_A_PIN, OUTPUT);
  // pinMode(LOCK_B_PIN, OUTPUT);
  // pinMode(LOCK_C_PIN, OUTPUT);
  // pinMode(LOCK_D_PIN, OUTPUT);
  // pinMode(LOCK_E_PIN, OUTPUT);

  pinMode(AO_STATE_PIN, INPUT_PULLUP);
  pinMode(CO_STATE_PIN, INPUT_PULLUP);
  pinMode(PT_STATE_PIN, INPUT_PULLUP);
  pinMode(BL_STATE_PIN, INPUT_PULLUP);

  // pinMode(SOUND_GOOD_TP, OUTPUT);
  // pinMode(SOUND_BAD_TP, OUTPUT);
  // pinMode(SOUND_WIN_TP, OUTPUT);
  // pinMode(SOUND_RES_1_TP, OUTPUT);
  // pinMode(SOUND_RES_2_TP, OUTPUT);
  // pinMode(SOUND_RES_3_TP, OUTPUT);
  // pinMode(SOUND_RES_4_TP, OUTPUT);
  // pinMode(SOUND_RES_5_TP, OUTPUT);

  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(LED3_PIN, OUTPUT);

  pinMode(BUZZER_PIN, OUTPUT);

  pinMode(LEDSTRIP_PIN, OUTPUT);

  //init_com();
  init_gamestate();
  // init_lockcontrol();
  init_input();
  //init_sound();
  init_ledcontrol();
  //init_lcd();
  init_ble();

  subroutine_timestamp = millis();
}

void loop() {
  // Heartbeat
    if(millis() - subroutine_timestamp > 100){
      subroutine_timestamp = millis();
      digitalWrite(13, !digitalRead(13));
    }

  tick_gamestate();
  // tick_locks();
  //tick_input();
  // tick_sound();
  tick_leds();
  tick_ble();
  //tick_com();
  //tick_lcd();
}