#ifndef __CTRL_DATE_H__
#define __CTRL_DATE_H__

#include "lcd_i2c.h"

typedef struct
{
    LCD_I2C *lcd;
} Ctrl_Date;

void
ctrl_date_init(Ctrl_Date *date, LCD_I2C *lcd, I2C_HandleTypeDef *hi2c);

void
ctrl_date_deinit(Ctrl_Date *date);


#endif /* __CTRL_DATE_H__ */
