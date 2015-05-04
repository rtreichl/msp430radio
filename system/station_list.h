/*
 * station_list.h
 *
 *  Created on: 08.04.2015
 *      Author: Richard
 */

//----------------------------------------------------------------------------------------
//
/// \brief In here there are functions declared to display the avaiable station
///	lists and also to add/remove stations to this list.
//
//----------------------------------------------------------------------------------------

#ifndef SYSTEM_STATION_LIST_H_
#define SYSTEM_STATION_LIST_H_

#include <stdint.h>
#include <radio/radio.h>
#include <system/station_list_struct.h>

uint8_t station_list_display(uint8_t action);
uint8_t station_list_handler(uint8_t *encoder_left_button, int8_t *encoder_left_count, uint8_t *encoder_right_button, int8_t *encoder_right_count, uint8_t entry_num);

#endif /* SYSTEM_STATION_LIST_H_ */
