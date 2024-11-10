#include "controller_lcd.h"
#include "time.h"

static const char *weekday[] =
{ "SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT" };

void
ctrl_lcd_init(Ctrl_LCD *lcd, I2C_HandleTypeDef *i2c)
{
    lcd_i2c_init(&(lcd->lcd), i2c, 0x4E, LCD_20x4, 10);
}

/*
 * 
    lcd_i2c_pos_printf(&(lcd), 0, 0, "WED");
    lcd_i2c_pos_printf(&(lcd), 12,0, "06.11.24");
    lcd_i2c_pos_printf(&(lcd), 6,1, "03:25:48");
    lcd_i2c_pos_printf(&(lcd), 0,2, "22*");
    lcd_i2c_pos_printf(&(lcd), 11,2, "SNOW -01*");
    lcd_i2c_pos_printf(&(lcd), 0,3, "ACOS FLOAT 12.11.24");
 */

void
ctrl_lcd_update_time(Ctrl_LCD *lcd, RTC_TimeTypeDef time)
{
    char str[9] = { 0 };
    get_rtc_time(time, str);
    lcd_i2c_pos_printf(&(lcd->lcd), 6, 1, "%s", str);
}

void
ctrl_lcd_update_date(Ctrl_LCD *lcd, RTC_DateTypeDef date)
{   
    lcd_i2c_pos_printf(&(lcd->lcd), 0, 0, "%s", weekday[date.WeekDay]);
    char str[9] = { 0 };
    get_rtc_date(date, str);
    lcd_i2c_pos_printf(&(lcd->lcd), 12, 0, "%s", str);
}

void
ctrl_lcd_update_temp(Ctrl_LCD *lcd, float temp)
{
    lcd_i2c_pos_printf(&(lcd->lcd), 0, 2, "%.1f*", temp);
}

void
ctrl_lcd_update_hum(Ctrl_LCD *lcd, float hum)
{
    lcd_i2c_pos_printf(&(lcd->lcd), 16, 2, "%.1f", hum);
}
