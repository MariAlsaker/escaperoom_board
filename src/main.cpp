#include <Arduino.h>
#include "defines.h"
#include "gamestate.h"
#include "input.h"
#include "lcd.h"
#include "led_control.h"
#include "ble.h"

uint32_t subroutine_timestamp = 0;

void setup() {

  Serial.begin(9600);

  pinMode(AO_STATE_PIN, INPUT_PULLUP);
  pinMode(CO_STATE_PIN, INPUT_PULLUP);
  pinMode(PT_STATE_PIN, INPUT_PULLUP);
  pinMode(BL_STATE_PIN, INPUT_PULLUP);

  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(LED3_PIN, OUTPUT);

  pinMode(BUZZER_PIN, OUTPUT);

  init_gamestate();
  init_input();
  init_ledcontrol();
  //init_lcd();
  init_ble();

  subroutine_timestamp = millis();
}

void loop() {
  // Heartbeat
    if(millis() - subroutine_timestamp > 100){
      subroutine_timestamp = millis();
    }

  tick_gamestate();
  // tick_locks();
  //tick_input();
  // tick_sound();
  tick_leds();
  tick_ble();
  //tick_lcd();
}