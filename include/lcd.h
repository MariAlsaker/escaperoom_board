#ifndef LCD_H
#define LCD_H

#include <stdint.h>

enum lcd_states
{
    L_IDLE,
    L_INPUT_D,
    L_INPUT_E,
    L_ERROR,
    L_SUCCESS,
    L_FINISH,
    L_DEBUG
};

void init_lcd(void);
void set_state_lcd(lcd_states newstate);
void debug_state_lcd(uint8_t* states, uint8_t len);
char* get_numberbufstart(void);
void tick_lcd(void);


#endif // LCD_H