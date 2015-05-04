/*
 * time.h
 *
 *  Created on: 28.03.2015
 *      Author: Richard
 */

//----------------------------------------------------------------------------------------
//
/// \brief In here the struct for the time is defined. There are also additional
///	functions which help formatting the struct into an array and one to set a time.
//
//----------------------------------------------------------------------------------------

#ifndef SYSTEM_TIME_H_
#define SYSTEM_TIME_H_

#include <stdint.h>
#include <settings/radio_configuration.h>

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

extern const uint8_t month_days[];

void time_update();
uint8_t time_date_to_array(char *str);
uint8_t time_time_to_array(char *str);
uint8_t time_set_time(uint8_t hour, uint8_t minute, uint8_t day, uint8_t month, uint8_t year, uint8_t valid);

#endif /* SYSTEM_TIME_H_ */
