#include "controller_date.h"

void
ctrl_date_init(Ctrl_Date *date, LCD_I2C *lcd, I2C_HandleTypeDef *hi2c)
{
    *date = (Ctrl_Date) { .lcd = lcd };
}
