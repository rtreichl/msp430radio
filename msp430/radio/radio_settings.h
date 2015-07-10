/*
 * radio_settings.h
 *
 *  Created on: 14.04.2015
 *      Author: Richard
 */

#ifndef SYSTEM_RADIO_SETTINGS_H_
#define SYSTEM_RADIO_SETTINGS_H_

#include <radio/radio.h>
#include <menu/menu.h>
#include <system/station_list.h>

#define RADIO_SETTINGS_RDS_VIEW 		MENU_FIRST_ENTRY
#define RADIO_SETTINGS_RSQ_VIEW 		MENU_THIRD_ENTRY
#define RADIO_SETTINGS_PIPTY_VIEW 		MENU_SECOND_ENTRY
#define RADIO_SETTINGS_BRIGTH_VIEW 		MENU_FOURTH_ENTRY

#define RADIO_SETTINGS_FREQ_SEEK_UP 	MENU_FIRST_ENTRY
#define RADIO_SETTINGS_FREQ_SEEK_DOWN 	MENU_SECOND_ENTRY
#define RADIO_SETTINGS_FREQ_CHOOSE 		MENU_THIRD_ENTRY

#define RADIO_SETTINGS_STATION_VIEW 	MENU_FIRST_ENTRY
#define RADIO_SETTINGS_SEEKING_UP 		MENU_SECOND_ENTRY
#define RADIO_SETTINGS_SEEKING_DOWN 	MENU_THIRD_ENTRY
#define RADIO_SETTINGS_STORE_STATION 	MENU_FOURTH_ENTRY
#define RADIO_SETTINGS_AUTO_SEARCH 		MENU_FIFTH_ENTRY

#define RADIO_SETTINGS_VOLUME_START		MENU_SECOND_ENTRY
#define RADIO_SETTINGS_MENU_VOL_TA 		MENU_THIRD_ENTRY

#define RADIO_SETTINGS_RESET_NO 		MENU_FIRST_ENTRY
#define RADIO_SETTINGS_RESET_YES 		MENU_SECOND_ENTRY

#define RADIO_SETTINGS_SOURCE_FM 		MENU_FIRST_ENTRY
#define RADIO_SETTINGS_SOURCE_AM 		MENU_SECOND_ENTRY
#define RADIO_SETTINGS_SOURCE_LINEIN 	MENU_THIRD_ENTRY

#define RADIO_SETTINGS_TA_TP_ON 		MENU_FIRST_ENTRY
#define RADIO_SETTINGS_TA_TP_OFF 		MENU_SECOND_ENTRY

#define RADIO_BACKLIGHT_AUTOMATIC		MENU_FIRST_ENTRY
#define RADIO_BACKLIGHT_MANUEL			MENU_SECOND_ENTRY

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
