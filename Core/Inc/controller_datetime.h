#ifndef __CTRL_DATETIME_H__
#define __CTRL_DATETIME_H__

#include "stm32f1xx_hal.h"
#include "stdbool.h"

typedef struct
{
    RTC_HandleTypeDef *rtc;
    RTC_DateTypeDef    date;
    RTC_TimeTypeDef    time;
    bool new_time;
    bool new_date;
} Ctrl_DateTime;

void
ctrl_datetime_init(Ctrl_DateTime *dt, RTC_HandleTypeDef *rtc);

void
ctrl_datetime_update(Ctrl_DateTime *dt);

void
ctrl_date_deinit(Ctrl_DateTime *dt);


#endif /* __CTRL_DATETIME_H__ */
