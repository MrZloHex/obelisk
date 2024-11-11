/**
  ******************************************************************************
  * @file    controller_lcd.h 
  * @author  MrZloHex
  ******************************************************************************
  */

#ifndef __CONTROLLER_LCD_H__
#define __CONTROLLER_LCD_H__

#include "lcd_i2c.h"
#include "controller_state.h"

typedef struct
{
    LCD_I2C lcd;
} Ctrl_LCD;

void
ctrl_lcd_init(Ctrl_LCD *lcd, I2C_HandleTypeDef *i2c);

void
ctrl_lcd_update_time(Ctrl_LCD *lcd, RTC_TimeTypeDef time);
void
ctrl_lcd_update_date(Ctrl_LCD *lcd, RTC_DateTypeDef date);
void
ctrl_lcd_update_temp(Ctrl_LCD *lcd, float temp);
void
ctrl_lcd_update_hum(Ctrl_LCD *lcd, float hum);
void
ctrl_lcd_update_state(Ctrl_LCD *lcd, Ctrl_State state);


#endif /* __CONTROLLER_LCD_H__ */
