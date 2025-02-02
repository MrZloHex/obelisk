#include "controller_weather.h"
#include "mjson.h"

void
ctrl_wthr_parse(Weather *wthr, const char *buf, const size_t size)
{
    double t_temp, t_feels, t_humidity, t_code;
    mjson_get_number(buf, size, "$.result.temp", &t_temp);
    mjson_get_number(buf, size, "$.result.feels", &t_feels);
    mjson_get_number(buf, size, "$.result.humidity", &t_humidity);
    mjson_get_number(buf, size, "$.result.code", &t_code);

    *wthr = (Weather)
    {
        .temp     = (int)t_temp,
        .feels    = (int)t_feels,
        .humidity = (int)t_humidity,
        .code     = (int)t_code
    };
}
