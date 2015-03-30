/*
 * time.c
 *
 *  Created on: 28.03.2015
 *      Author: Richard
 */

#include <system/time.h>

#define time_month(x)	(x->month_ten * 10 + x->month_one)
#define time_day(x) 	(x->day_ten * 10 + x->day_one)
#define time_year(x) 	(x->year_ten * 10 + x->year_one)
#define time_hour(x) 	(x->hour_ten * 10 + x->hour_one)
#define time_minute(x) 	(x->minute_ten * 10 + x->minute_one)

static TIME_DATE *time_date;

const uint8_t month_days[12] = {
		31,
		28,
		31,
		30,
		31,
		30,
		31,
		31,
		30,
		31,
		30,
		31
};

uint8_t time_set(uint8_t hour, uint8_t minute, uint8_t day, uint8_t month, uint8_t year, uint8_t valid)
{
	time_date->hour_one = hour % 10;
	time_date->hour_ten = hour / 10;
	time_date->minute_one = minute % 10;
	time_date->minute_ten = minute / 10;
	time_date->day_one = day % 10;
	time_date->day_ten = day / 10;
	time_date->month_one = month % 10;
	time_date->month_ten= month / 10;
	time_date->year_one = year % 10;
	time_date->year_ten= year / 10;
	return 0;
}

uint8_t time_to_str(char *str)
{
	str[0] = time_date->hour_ten;
	str[1] = time_date->hour_one;
	str[2] = ':';
	str[3] = time_date->minute_ten;
	str[4] = time_date->minute_one;
	str[5] = 0;

	return 0;
}

uint8_t date_to_str(char *str)
{
	str[0] = time_date->day_ten;
	str[1] = time_date->day_one;
	str[2] = '.';
	str[3] = time_date->month_ten;
	str[4] = time_date->month_one;
	str[5] = '.';
	str[6] = time_date->year_ten;
	str[7] = time_date->year_one;
	str[8] = 0;

	return 0;
}

void time_date_update()
{

	time_date->minute_one++;
	if (time_date->minute_one >= 10) {
		time_date->minute_one -= 10;
		time_date->minute_ten++;
		if (time_date->minute_ten >= 6) {
			time_date->minute_ten -= 6;
			time_date->hour_one++;
			if (time_date->hour_one >= 10) {
				time_date->hour_one -= 10;
				time_date->hour_ten++;
			}
			if (time_hour(time_date) == 24) {
				time_date->hour_one = 0;
				time_date->hour_ten = 0;
				time_date->day_one++;
				if (time_date->day_one >= 10) {
					time_date->day_one -= 10;
					time_date->day_ten++;
				}
				if (time_day(time_date) - 1 >= month_days[time_month(time_date) - 1]) {
					if (!(time_month(time_date) == 2 && time_year(time_date) % 4 == 0 && time_day(time_date) - 1 == 28)) {
						time_date->day_one = 1;
						time_date->day_ten = 0;
						time_date->month_one++;
					}
					if (time_date->month_one >= 10) {
						time_date->month_one -= 10;
						time_date->month_ten++;
					}
					if (time_month(time_date) == 13) {
						time_date->month_ten = 0;
						time_date->month_one -= 2;
						time_date->year_one++;
						if (time_date->year_one == 10) {
							time_date->year_one -= 10;
							time_date->year_ten++;
							if (time_date->year_ten == 10) {
								time_date->year_ten -= 10;
							}
						}
					}
				}
			}
		}
	}
}
