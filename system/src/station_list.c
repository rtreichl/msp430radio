/*
 * station_list_display.c
 *
 *  Created on: 08.04.2015
 *      Author: Richard
 */

#include <system/station_list.h>

STATION_LIST *actuall_station = &station_1;

uint8_t station_list_handler(uint8_t *encoder_right_button, int8_t *encoder_right_count, uint8_t action)
{
	if (*encoder_right_count != 0) {
		if(*encoder_right_count > 0) {
			if (actuall_station->next != 0) {
				actuall_station = actuall_station->next;
			}
		}
		if (*encoder_right_count < 0) {
			if (actuall_station->previous != 0) {
				actuall_station = actuall_station->previous;
			}
		}
		*encoder_right_count = 0;
	}
	if(*encoder_right_button == BUTTON_PRESS_SHORT) {
		switch(action) {
		case STORE_STATION:
			radio_store_station(&(radio.settings.frequency), radio.rds.name, actuall_station->entry_num - 1);
			return SHORT_UP_TO_PARENT;
		case STATION_VIEW:
			radio.settings.frequency = *(actuall_station->freq);
			radio_tune_freq(*(actuall_station->freq));
			return SHORT_UP_TO_PARENT;
		case MENU_FREQ_CHOOSE_ENTRY:
			radio.settings.frequency = *(actuall_station->freq);
			radio_store_settings(1, 0);
			return SHORT_UP_TO_PARENT;

		}
	}
	station_list_display(action);
	return 0;
}

uint8_t station_list_display(uint8_t action)
{

	if (actuall_station->previous != 0) {
		lcd_create_view(actuall_station->previous->text, 1, 0, 8, 0);
	}
	else if(action == STATION_VIEW) {
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
