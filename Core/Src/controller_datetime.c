#include "controller_datetime.h"
#include "stdbool.h"

static bool
same_date(RTC_DateTypeDef d1, RTC_DateTypeDef d2)
{
    return (d1.Year  == d2.Year)  &&
           (d1.Month == d2.Month) &&
           (d1.Date  == d2.Date);
}

static bool
same_time(RTC_TimeTypeDef t1, RTC_TimeTypeDef t2)
{
    return (t1.Hours   == t2.Hours)   &&
           (t1.Minutes == t2.Minutes) &&
           (t1.Seconds == t2.Seconds);
}

void
ctrl_datetime_init(Ctrl_DateTime *dt, RTC_HandleTypeDef *rtc)
{
    *dt = (Ctrl_DateTime) { .rtc = rtc };

    RTC_DateTypeDef date = 
    {
        .Month = 11,
        .Date  = 10,
        .Year  = 24,
        .WeekDay = 0
    };
    RTC_TimeTypeDef time =
    {
        .Hours   = 18,
        .Minutes = 14,
        .Seconds = 0
    };
    
    HAL_RTC_SetTime(rtc, &time, RTC_FORMAT_BIN);
    HAL_RTC_SetDate(rtc, &date, RTC_FORMAT_BIN);

    date = (RTC_DateTypeDef) { 0 };
    time = (RTC_TimeTypeDef) { 0 };
    HAL_RTC_GetTime(dt->rtc, &time, RTC_FORMAT_BIN);
    HAL_RTC_GetDate(dt->rtc, &date, RTC_FORMAT_BIN);

    dt->date = date;
    dt->time = time;
}

void
ctrl_datetime_update(Ctrl_DateTime *dt)
{
    RTC_DateTypeDef date = { 0 };
    RTC_TimeTypeDef time = { 0 };
    HAL_RTC_GetTime(dt->rtc, &time, RTC_FORMAT_BIN);
    HAL_RTC_GetDate(dt->rtc, &date, RTC_FORMAT_BIN);

    dt->new_date = true;
    dt->new_time = true;
    if (!same_date(date, dt->date))
    {
        dt->new_date = false;
        dt->date = date;
    }
    if (!same_time(time, dt->time))
    {
        dt->new_time = false;
        dt->time = time;
    }
}

