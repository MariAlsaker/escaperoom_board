#ifndef LED_CONTROL_H
#define LED_CONTROL_H

#include <stdint.h>

#define LED_COUNT 2

enum led_index {
    LED_G = 0,
    LED_R = 1
};

enum led_mode {
    LED_OFF = 0,
    LED_BLINK = 1,
    LED_PULSE = 2,
    LED_FADEOUT = 3,
    LED_FADEIN = 4,
    LED_ON = 5
};

typedef struct {
    enum led_mode mode;

    //Used for blink, pulse and fade
    int16_t power;

    uint8_t increasing;
    uint8_t intensity;
    uint8_t speed;

    uint32_t timer;
    uint8_t pin;

    uint32_t software_pwm_timer;
} led_t;

void init_ledcontrol(void);
void set_ledmode(enum led_index led, enum led_mode mode);
void tick_leds(void);

#endif