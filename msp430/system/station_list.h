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

#include <menu/menu.h>

uint8_t station_list_display(uint8_t action);
uint8_t station_list_handler(ENCODER *encoder_left, ENCODER *encoder_right, MENU_STC *menu);

#endif /* SYSTEM_STATION_LIST_H_ */
