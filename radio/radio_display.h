/*
 * radio_display.h
 *
 *  Created on: 08.04.2015
 *      Author: Richard
 */

#ifndef SYSTEM_RADIO_DISPLAY_H_
#define SYSTEM_RADIO_DISPLAY_H_

#include <stdint.h>
#include <radio/radio.h>

//----------------------------------------------------------------------------------------
//
/// \brief Definition of functions for:\n
///	-Display the speaker symbol\n
///	-Display RDS\n
///	-Display PI/PTY\n
///	-Display RSQ\n
///	-the display handler\n
//
//----------------------------------------------------------------------------------------

uint8_t radio_display_speaker();
uint8_t radio_display_rds();
uint8_t radio_display_pipty();
uint8_t radio_display_rsq();
uint8_t radio_display_handler(uint8_t blend_scroll, uint8_t value);

#endif /* SYSTEM_RADIO_DISPLAY_H_ */
