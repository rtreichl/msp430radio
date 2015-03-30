/*
 * time.h
 *
 *  Created on: 28.03.2015
 *      Author: Richard
 */

#ifndef SYSTEM_TIME_H_
#define SYSTEM_TIME_H_

#include <stdint.h>

extern const uint8_t month_days[];

typedef struct time_date {
	uint32_t minute_one:4;
	uint32_t minute_ten:3;
	uint32_t hour_one:4;
	uint32_t hour_ten:2;
	uint32_t day_one:4;
	uint32_t day_ten:2;
	uint32_t month_one:4;
	uint32_t month_ten:1;
	uint32_t year_one:4;
	uint32_t year_ten:4;
} TIME_DATE;

void time_date_update();
uint8_t date_to_str(char *str);
uint8_t time_to_str(char *str);
uint8_t time_set(uint8_t hour, uint8_t minute, uint8_t day, uint8_t month, uint8_t year, uint8_t valid);

#endif /* SYSTEM_TIME_H_ */
