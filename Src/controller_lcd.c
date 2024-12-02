#include "controller_lcd.h"
#include "time.h"

static const char *weekday[] =
{
    "Sunday   ", "Monday   ", "Tuesday  ", "Wednesday",
    "Thursday ", "Friday   ", "Saturday "
};

uint8_t HeartChar[] = {0x00, 0x00, 0x0a, 0x15, 0x11, 0x0a, 0x04, 0x00};
uint8_t degree[] = {0x18,0x18,0x03,0x04,0x04,0x04,0x03,0x00};
uint8_t online[] = {0x10,0x18,0x1C,0x19,0x13,0x07,0x03,0x01};
uint8_t synced[] = {0x0C,0x10,0x08,0x05,0x1D,0x02,0x02,0x02};
uint8_t hv[] = { 0b10000, 0b10010, 0b10001, 0b11001, 0b10101, 0b10101, 0b10010, 0b00000 };



uint8_t sunny[] = {0x18,0x1B,0x00,0x0A,0x09,0x00,0x00,0x00};
uint8_t partly[] = {0x18,0x18,0x03,0x07,0x0F,0x0F,0x00,0x00};
uint8_t cloudly[] = {0x06,0x0F,0x1F,0x1F,0x00,0x00,0x00,0x00};
uint8_t snowy[] = {0x06,0x0F,0x1F,0x1F,0x11,0x04,0x12,0x04};
uint8_t thunder[] = {0x06,0x0F,0x1F,0x1F,0x04,0x08,0x04,0x08};


void
ctrl_lcd_init(Ctrl_LCD *lcd, I2C_HandleTypeDef *i2c)
{
    lcd_i2c_init(&(lcd->lcd), i2c, 0x4E, LCD_20x4, 10);

    lcd_i2c_make_custom_char(&(lcd->lcd), 0, degree);
    lcd_i2c_make_custom_char(&(lcd->lcd), 1, online);
    lcd_i2c_make_custom_char(&(lcd->lcd), 2, synced);

    lcd_i2c_make_custom_char(&(lcd->lcd), 3, sunny);
    lcd_i2c_make_custom_char(&(lcd->lcd), 4, partly);
    lcd_i2c_make_custom_char(&(lcd->lcd), 5, cloudly);
    lcd_i2c_make_custom_char(&(lcd->lcd), 6, snowy);
    lcd_i2c_make_custom_char(&(lcd->lcd), 7, thunder);

    lcd_i2c_pos_printf(&(lcd->lcd), 6, 3, "MrZloHex");


    // lcd_i2c_pos_char(&(lcd->lcd), 8, 2, 3);
    // lcd_i2c_char(&(lcd->lcd), 5);
    // lcd_i2c_char(&(lcd->lcd), 6);
    // lcd_i2c_char(&(lcd->lcd), 7);

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
ctrl_lcd_update_temp(Ctrl_LCD *lcd, int temp, int feels)
{
    lcd_i2c_pos_printf(&(lcd->lcd), 0, 2, "%d(%d)", temp, feels);
    lcd_i2c_char(&(lcd->lcd), 0);
    lcd_i2c_printf(&(lcd->lcd), "  ");

}

void
ctrl_lcd_update_hum(Ctrl_LCD *lcd, int hum)
{
    lcd_i2c_pos_printf(&(lcd->lcd), 17, 2, "%d%%", hum);
}

static int
ctrl_lcd_calc_wthr_icon(int code)
{
    if (code == 113) { return 3; }
    if (code == 116) { return 4; }
    if (code == 119) { return 5; }
    if (code == 122) { return 5; }
    if (code == 143) { return 5; }
    if (code == 176) { return 6; }
    if (code == 179) { return 6; }
    if (code == 182) { return 6; }
    if (code == 185) { return 6; }
    if (code == 200) { return 7; }
    if (code == 227) { return 6; }
    if (code == 230) { return 6; }
    if (code == 248) { return 5; }
    if (code == 260) { return 5; }
    if (code == 263) { return 6; }
    if (code == 266) { return 6; }
    if (code == 281) { return 6; }
    if (code == 284) { return 6; }
    if (code == 293) { return 6; }
    if (code == 296) { return 6; }
    if (code == 299) { return 6; }
    if (code == 302) { return 6; }
    if (code == 305) { return 6; }
    if (code == 308) { return 6; }
    if (code == 311) { return 6; }
    if (code == 314) { return 6; }
    if (code == 317) { return 6; }
    if (code == 320) { return 6; }
    if (code == 323) { return 6; }
    if (code == 326) { return 6; }
    if (code == 329) { return 6; }
    if (code == 332) { return 6; }
    if (code == 335) { return 6; }
    if (code == 338) { return 6; }
    if (code == 350) { return 6; }
    if (code == 353) { return 6; }
    if (code == 356) { return 6; }
    if (code == 359) { return 6; }
    if (code == 362) { return 6; }
    if (code == 365) { return 6; }
    if (code == 368) { return 6; }
    if (code == 371) { return 6; }
    if (code == 374) { return 6; }
    if (code == 377) { return 6; }
    if (code == 386) { return 7; }
    if (code == 389) { return 7; }
    if (code == 392) { return 7; }
    if (code == 395) { return 7; }
}


void
ctrl_lcd_update_wthr(Ctrl_LCD *lcd, Weather wthr)
{
    ctrl_lcd_update_temp(lcd, wthr.temp, wthr.feels);
    ctrl_lcd_update_hum(lcd, wthr.humidity);
    lcd_i2c_pos_char(&(lcd->lcd), 15, 2, ctrl_lcd_calc_wthr_icon(wthr.code));
}

void
ctrl_lcd_update_state(Ctrl_LCD *lcd, Ctrl_State state)
{
    lcd_i2c_pos_char(&(lcd->lcd), 0,  1, state.connected ? 1 : ' ');
    lcd_i2c_pos_char(&(lcd->lcd), 19, 1, state.synced    ? 2 : ' ');
}
