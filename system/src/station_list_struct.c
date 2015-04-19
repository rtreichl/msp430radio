/*
 * station_list.c
 *
 *  Created on: 07.04.2015
 *      Author: Richard
 */


#include <system/station_list_struct.h>

//----------------------------------------------------------------------------------------
//
/// \description Definition of the 14 station lists
//
//----------------------------------------------------------------------------------------

STATION_LIST station_1 = {
		&station_2,
		0,
		(uint16_t *) RADIO_STATION_FREQ_STORE_ADR + 0,
		(char *) RADIO_STATION_NAME_STORE_ADR + RADIO_STATION_NAME_STORE_SIZE * 0,
		1,
};

STATION_LIST station_2 = {
		&station_3,
		&station_1,
		(uint16_t *) RADIO_STATION_FREQ_STORE_ADR + 1,
		(char *) RADIO_STATION_NAME_STORE_ADR + RADIO_STATION_NAME_STORE_SIZE * 1,
		2,
};

STATION_LIST station_3 = {
		&station_4,
		&station_2,
		(uint16_t *) RADIO_STATION_FREQ_STORE_ADR + 2,
		(char *) RADIO_STATION_NAME_STORE_ADR + RADIO_STATION_NAME_STORE_SIZE * 2,
		3,
};

STATION_LIST station_4 = {
		&station_5,
		&station_3,
		(uint16_t *) RADIO_STATION_FREQ_STORE_ADR + 3,
		(char *) RADIO_STATION_NAME_STORE_ADR + RADIO_STATION_NAME_STORE_SIZE * 3,
		4,
};

STATION_LIST station_5 = {
		&station_6,
		&station_4,
		(uint16_t *) RADIO_STATION_FREQ_STORE_ADR + 4,
		(char *) RADIO_STATION_NAME_STORE_ADR + RADIO_STATION_NAME_STORE_SIZE * 4,
		5,
};

STATION_LIST station_6 = {
		&station_7,
		&station_5,
		(uint16_t *) RADIO_STATION_FREQ_STORE_ADR + 5,
		(char *) RADIO_STATION_NAME_STORE_ADR + RADIO_STATION_NAME_STORE_SIZE * 5,
		6,
};

STATION_LIST station_7 = {
		&station_8,
		&station_6,
		(uint16_t *) RADIO_STATION_FREQ_STORE_ADR + 6,
		(char *) RADIO_STATION_NAME_STORE_ADR + RADIO_STATION_NAME_STORE_SIZE * 6,
		7,
};

STATION_LIST station_8 = {
		&station_9,
		&station_7,
		(uint16_t *) RADIO_STATION_FREQ_STORE_ADR + 7,
		(char *) RADIO_STATION_NAME_STORE_ADR + RADIO_STATION_NAME_STORE_SIZE * 7,
		8,
};

STATION_LIST station_9 = {
		&station_10,
		&station_8,
		(uint16_t *) RADIO_STATION_FREQ_STORE_ADR + 8,
		(char *) RADIO_STATION_NAME_STORE_ADR + RADIO_STATION_NAME_STORE_SIZE * 8,
		9,
};

STATION_LIST station_10 = {
		&station_11,
		&station_9,
		(uint16_t *) RADIO_STATION_FREQ_STORE_ADR + 9,
		(char *) RADIO_STATION_NAME_STORE_ADR + RADIO_STATION_NAME_STORE_SIZE * 9,
		10,
};

STATION_LIST station_11 = {
		&station_12,
		&station_10,
		(uint16_t *) RADIO_STATION_FREQ_STORE_ADR + 10,
		(char *) RADIO_STATION_NAME_STORE_ADR + RADIO_STATION_NAME_STORE_SIZE * 10,
		11,
};

STATION_LIST station_12 = {
		&station_13,
		&station_11,
		(uint16_t *) RADIO_STATION_FREQ_STORE_ADR + 11,
		(char *) RADIO_STATION_NAME_STORE_ADR + RADIO_STATION_NAME_STORE_SIZE * 11,
		12,
};

STATION_LIST station_13 = {
		&station_14,
		&station_12,
		(uint16_t *) RADIO_STATION_FREQ_STORE_ADR + 12,
		(char *) RADIO_STATION_NAME_STORE_ADR + RADIO_STATION_NAME_STORE_SIZE * 12,
		13,
};

STATION_LIST station_14 = {
		0,
		&station_13,
		(uint16_t *) RADIO_STATION_FREQ_STORE_ADR + 13,
		(char *) RADIO_STATION_NAME_STORE_ADR + RADIO_STATION_NAME_STORE_SIZE * 13,
		14,
};
