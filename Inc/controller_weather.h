#ifndef __CTRL_WEATHER_H__
#define __CTRL_WEATHER_H__

#include <stdlib.h>

typedef struct
{
    int temp;
    int feels;
    int humidity;
    int code;
} Weather;

void
ctrl_wthr_parse(Weather *wthr, const char *buf, const size_t size);

#endif /* __CTRL_WEATHER_H__ */
