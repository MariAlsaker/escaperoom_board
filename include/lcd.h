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
    L_FINISH
};

void init_lcd(void);
void set_state_lcd(lcd_states newstate);
char* get_numberbufstart(void);
void tick_lcd(void);


#endif // LCD_H