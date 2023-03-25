#ifndef LCD_H
#define LCD_H

enum lcd_states
{
    L_STARTUP,
    L_INFO,
    L_INFO_SNEEZE,
    L_INFO_SNEEZE_THANKS,
    L_MENU //Future
};

void init_lcd(void);
void tick_lcd(void);


#endif // LCD_H