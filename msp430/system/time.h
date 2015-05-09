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
	uint32_t minute:6;		//6bit
	uint32_t hour:5;		//5bit
	uint32_t day:5;			//5bit
	uint32_t month:4;		//4bit
	uint32_t year:7;		//6bit
} TIME_DATE;

extern const uint8_t month_days[];

void time_update();
uint8_t time_date_to_array(char *str);
uint8_t time_time_to_array(char *str);
uint8_t time_set_time(uint8_t hour, uint8_t minute, uint8_t day, uint8_t month, uint8_t year, uint8_t valid);

#endif /* SYSTEM_TIME_H_ */
