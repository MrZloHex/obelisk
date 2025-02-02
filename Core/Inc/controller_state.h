#ifndef __CTRL_STATE_H__
#define __CTRL_STATE_H__

#include <stdbool.h>
#include "time.h"
#include "controller_weather.h"

typedef struct
{
    bool connected;
    bool synced;
    bool wthr_synced;
    RTC_HandleTypeDef *rtc;
    Weather weather;
} Ctrl_State;

#endif /* __CTRL_STATE_H__  */
