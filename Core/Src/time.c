#include "time.h"
#include "mjson.h"
#include "convert.h"
#include "string.h"


static int
zero_2_int
(
	mjson_print_fn_t fn,
	void *fndata,
	va_list *ap
)
{
	int value = va_arg(*ap, int);
	if (value < 10)
		return mjson_printf(fn, fndata, "0%u", value);
	else
		return mjson_printf(fn, fndata, "%u", value);
}


void
calculate_weekday(RTC_DateTypeDef *date)
{
    uint8_t month = date->Month;
    uint16_t year = date->Year + 2000U;
    uint8_t day   = date->Date;

    if (month < 3) {
        month += 12;
        year -= 1;
    }
    uint16_t c = year / 100;
    year = year % 100;
    int h = (c / 4 - 2 * c + year + year / 4 + 13 * (month + 1) / 5 + day - 1) % 7;

    date->WeekDay = (h + 7) % 7;
}


void
adjust_rtc_time
(
	RTC_HandleTypeDef *rtc,
	char str_time[25]
)
{
	RTC_TimeTypeDef time = {0};
	RTC_DateTypeDef date = {0};

	// 2022-10-08T14:50:37.116Z
	char year_str[3] = {0};
	strncpy(year_str, str_time+2, 2);
	charset_to_uint8(&date.Year, year_str);

	char month_str[3] = {0};
	strncpy(month_str, str_time+5, 2);
	charset_to_uint8(&date.Month, month_str);

	char day_str[3] = {0};
	strncpy(day_str, str_time+8, 2);
	charset_to_uint8(&date.Date, day_str);

	char hours_str[3] = {0};
	strncpy(hours_str, str_time+11, 2);
	charset_to_uint8(&time.Hours, hours_str);

	char minutes_str[3] = {0};
	strncpy(minutes_str, str_time+14, 2);
	charset_to_uint8(&time.Minutes, minutes_str);

	char seconds_str[3] = {0};
	strncpy(seconds_str, str_time+17, 2);
	charset_to_uint8(&time.Seconds, seconds_str);

    calculate_weekday(&date);
	
	
	HAL_RTC_SetTime
	(
		rtc,
		&time,
		RTC_FORMAT_BIN
	);

	HAL_RTC_SetDate
	(
		rtc,
		&date,
		RTC_FORMAT_BIN
	);
}

void
get_rtc_date(RTC_DateTypeDef date, char str[9])
{
	mjson_snprintf
	(
		str, 9,
		"%M.%M.%M",
		zero_2_int, date.Date,
		zero_2_int, date.Month,
		zero_2_int, date.Year
	);
}

void
get_rtc_time(RTC_TimeTypeDef time, char str[9])
{
	mjson_snprintf
	(
		str, 9,
		"%M:%M:%M",
		zero_2_int, time.Hours,
		zero_2_int, time.Minutes,
		zero_2_int, time.Seconds
	);
}

void
get_rtc_datetime
(
	RTC_HandleTypeDef *rtc,
	char str_time[25]
)
{
	RTC_TimeTypeDef time = {0};
	RTC_DateTypeDef date = {0};

	HAL_RTC_GetTime
	(
		rtc,
		&time,
		RTC_FORMAT_BIN
	);

	HAL_RTC_GetDate
	(
		rtc,
		&date,
		RTC_FORMAT_BIN
	);

	mjson_snprintf
	(
		str_time, 25,
		"20%u-%M-%MT%M:%M:%M.000Z",
		date.Year,
		zero_2_int, date.Month,
		zero_2_int, date.Date,
		zero_2_int, time.Hours,
		zero_2_int, time.Minutes,
		zero_2_int, time.Seconds
	);
}
