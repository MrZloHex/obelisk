#include "controller_lcd.h"
#include "time.h"

static const char *weekday[] =
{
    "Sunday   ", "Monday   ", "Tuesday  ", "Wednesday",
    "Thursday ", "Friday   ", "Saturday "
};

uint8_t HeartChar[] = {0x00, 0x00, 0x0a, 0x15, 0x11, 0x0a, 0x04, 0x00};
uint8_t degree[] = {0x1C,0x14,0x1C,0x00,0x00,0x00,0x00,0x00};
uint8_t online[] = {0x10,0x18,0x1C,0x19,0x13,0x07,0x03,0x01};
uint8_t synced[] = {0x0C,0x10,0x08,0x05,0x1D,0x02,0x02,0x02};
uint8_t hv[] = { 0b10000, 0b10010, 0b10001, 0b11001, 0b10101, 0b10101, 0b10010, 0b00000 };


void
ctrl_lcd_init(Ctrl_LCD *lcd, I2C_HandleTypeDef *i2c)
{
    lcd_i2c_init(&(lcd->lcd), i2c, 0x4E, LCD_20x4, 10);

    // lcd_i2c_make_custom_char(&(lcd->lcd), 0, HeartChar);
    lcd_i2c_make_custom_char(&(lcd->lcd), 0, degree);
    lcd_i2c_make_custom_char(&(lcd->lcd), 1, hv);
    lcd_i2c_make_custom_char(&(lcd->lcd), 2, online);
    lcd_i2c_make_custom_char(&(lcd->lcd), 3, synced);

    lcd_i2c_pos_char(&(lcd->lcd), 4, 3, 1);
    lcd_i2c_pos_printf(&(lcd->lcd), 6, 3, "MrZloHex");
    lcd_i2c_pos_char(&(lcd->lcd), 15, 3, 1);
}

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
    lcd_i2c_pos_printf(&(lcd->lcd), 0, 2, "%.1f", temp);
    lcd_i2c_pos_char(&(lcd->lcd), 4, 2, 0);
}

void
ctrl_lcd_update_hum(Ctrl_LCD *lcd, float hum)
{
    lcd_i2c_pos_printf(&(lcd->lcd), 15, 2, "%2.1f%%", hum);
}

void
ctrl_lcd_update_state(Ctrl_LCD *lcd, Ctrl_State state)
{
    lcd_i2c_pos_char(&(lcd->lcd), 0,  1, state.connected ? 2 : ' ');
    lcd_i2c_pos_char(&(lcd->lcd), 19, 1, state.synced    ? 3 : ' ');
}
