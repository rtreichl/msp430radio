/*
 * station_list.h
 *
 *  Created on: 07.04.2015
 *      Author: Richard
 */

//----------------------------------------------------------------------------------------
//
/// \brief In here the station_list struct is declared. There are 14 places for
///	stations to be stored.
//
//----------------------------------------------------------------------------------------

#ifndef SYSTEM_STATION_LIST_STRUCT_H_
#define SYSTEM_STATION_LIST_STRUCT_H_

#include <stdint.h>
#include <radio/radio.h>

typedef const struct station_list {
	const struct station_list *next;
	const struct station_list *previous;
	const uint16_t *freq;
	const char *text;
	uint8_t entry_num;
}STATION_LIST;

extern STATION_LIST station_1;
extern STATION_LIST station_2;
extern STATION_LIST station_3;
extern STATION_LIST station_4;
extern STATION_LIST station_5;
extern STATION_LIST station_6;
extern STATION_LIST station_7;
extern STATION_LIST station_8;
extern STATION_LIST station_9;
extern STATION_LIST station_10;
extern STATION_LIST station_11;
extern STATION_LIST station_12;
extern STATION_LIST station_13;
extern STATION_LIST station_14;

#endif /* SYSTEM_STATION_LIST_STRUCT_H_ */
