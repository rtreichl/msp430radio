/*
 * radio_settings.h
 *
 *  Created on: 14.04.2015
 *      Author: Richard
 */

#ifndef SYSTEM_RADIO_SETTINGS_H_
#define SYSTEM_RADIO_SETTINGS_H_

#include <radio/radio.h>
#include <system/station_list.h>

//----------------------------------------------------------------------------------------
//
/// \brief Definition of functions for:\n
///	-Set the brightness\n
///	-Set the contrast\n
///	-Set the source\n
///	-Set the Equalizer Mode\n
///	-Set the volume\n
///	-Set the frequency\n
///	-Choose the display data\n
///	-Seeks the next station/all available\n
///	-Set TA/TP\n
///	-Reset settings\n
//
//----------------------------------------------------------------------------------------

uint8_t radio_settings_brightness(ENCODER *encoder_left, ENCODER *encoder_right, MENU_STC *menu);
uint8_t radio_settings_contrast(ENCODER *encoder_left, ENCODER *encoder_right, MENU_STC *menu);
uint8_t radio_settings_source(ENCODER *encoder_left, ENCODER *encoder_right, MENU_STC *menu);
uint8_t radio_settings_equalizer(ENCODER *encoder_left, ENCODER *encoder_right, MENU_STC *menu);
uint8_t radio_settings_volume(ENCODER *encoder_left, ENCODER *encoder_right, MENU_STC *menu);
uint8_t radio_settings_frequency(ENCODER *encoder_left, ENCODER *encoder_right, MENU_STC *menu);
uint8_t radio_settings_view(ENCODER *encoder_left, ENCODER *encoder_right, MENU_STC *menu);
uint8_t radio_settings_station(ENCODER *encoder_left, ENCODER *encoder_right, MENU_STC *menu);
uint8_t radio_settings_tatp(ENCODER *encoder_left, ENCODER *encoder_right, MENU_STC *menu);
uint8_t radio_settings_reset(ENCODER *encoder_left, ENCODER *encoder_right, MENU_STC *menu);

#endif /* SYSTEM_RADIO_SETTINGS_H_ */
