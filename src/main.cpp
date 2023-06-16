#include <Arduino.h>
#include "defines.h"
#include "gamestate.h"
#include "input.h"
#include "lcd.h"
#include "led_control.h"
#include "ble.h"

uint32_t subroutine_timestamp = 0;

void setup() {

  Serial.begin(115200);

  pinMode(AO_STATE_PIN, INPUT_PULLUP);
  pinMode(PT_STATE_PIN, INPUT_PULLUP);
  pinMode(PT_STATE_PIN_ERROR, INPUT_PULLUP);
  pinMode(BL_STATE_PIN, INPUT_PULLUP);

  pinMode(REDLED_PIN, OUTPUT);
  pinMode(GREENLED_PIN, OUTPUT);

  init_gamestate();
  init_input();
  init_ledcontrol();
  init_lcd();
  init_ble();

  poll_debug_gamestate();

  subroutine_timestamp = millis();
}

void loop() {
  // Heartbeat
    if(millis() - subroutine_timestamp > 100){
      subroutine_timestamp = millis();
    }

  tick_gamestate();
  tick_input();
  tick_lcd();
  tick_leds();
  tick_ble();
}