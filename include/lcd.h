#ifndef LCD_H
#define LCD_H

enum lcd_states
{
    L_IDLE,
    L_INPUT,
    L_ERROR,
    L_SUCCESS
};

void init_lcd(void);
void tick_lcd(void);


#endif // LCD_H