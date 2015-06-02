/*
 * station_list_display.c
 *
 *  Created on: 08.04.2015
 *      Author: Richard
 */

#include <system/station_list.h>
#include <system/station_list_struct.h>

STATION_LIST *actuall_station = &station_1;

//----------------------------------------------------------------------------------------
//
/// \brief Handler to select the current station
//
/// Following steps are done to get the current selected station:\n
///	(1)Update the current station according to the encoder count\n
///	(2)If the button was pressed for a short time-> actualize current station\n
///	(3)Update the display\n
//
/// \param	<encoder_left_button>		[in]	state of the left encoder button
/// \param	<encoder_left_count>		[in]	value of the left encoder
/// \param	<encoder_right_button>		[in]	state of the right encoder button
/// \param	<encoder_right_count>		[in]	value of the right encoder
/// \param	<entry_num>					[in]	the entry in the menu
//
/// \retval uint8_t
//
/// \remarks
//
//----------------------------------------------------------------------------------------

uint8_t station_list_handler(ENCODER *encoder_left, ENCODER *encoder_right, MENU_STC *menu)
{
	if (encoder_right->count != 0) {
		if(encoder_right->count > 0) {
			if (actuall_station->next != 0) {
				actuall_station = actuall_station->next;
			}
		}
		if (encoder_right->count < 0) {
			if (actuall_station->previous != 0) {
				actuall_station = actuall_station->previous;
			}
		}
		encoder_right->count = 0;
	}
	if(*encoder_right->button == BUTTON_SHORT) {
		if(*(actuall_station->freq) >= RADIO_BOT_FREQ) {
			switch(menu->y) {
			case MENU_FOURTH_ENTRY:
				radio_store_station(&(radio.settings.frequency), radio.rds.name, actuall_station->entry_num - 1);
				return SHORT_UP_TO_PARENT;
			case MENU_FIRST_ENTRY:
				radio.settings.frequency = *(actuall_station->freq);
				radio_tune_freq(*(actuall_station->freq));
				return SHORT_UP_TO_PARENT;
			case MENU_THIRD_ENTRY:
				radio.settings.frequency = *(actuall_station->freq);
				radio_store_settings(1, 0);
				return SHORT_UP_TO_PARENT;

			}
		}
		return SHORT_UP_TO_CHILD;
	}

	if(*encoder_left->button == BUTTON_SHORT) {
		return SHORT_UP_TO_CHILD;
	}

	station_list_display(menu->y);
	return 0;
}

//TODO: rework required
//----------------------------------------------------------------------------------------
//
/// \brief	Displays the station list
//
/// When updating the station_list on the display it depends on the current \n
///	station or action the user wants to do.
//
/// \param	<action>	[in]	Action to be done
//
/// \retval	uint8_t
//
/// \remarks
//
//----------------------------------------------------------------------------------------

uint8_t station_list_display(uint8_t action)
{

	if (actuall_station->previous != 0) {
		lcd_create_view(actuall_station->previous->text, 1, 0, 8, 0);
	}
	else if(action == MENU_FIRST_ENTRY) {
		lcd_create_view(choose_text, 0, 0, 0, 0);
	}
	else {
		lcd_create_view(save_text, 0, 0, 0, 0);
	}
	lcd_create_view(actuall_station->text, 1, 1, 8, 0);
	if (actuall_station->next != 0) {
		lcd_create_view(actuall_station->next->text, 1, 2, 8, 0);
	}
	lcd_create_view("~", 0, 1, 0, 1);

	return 0;
}
