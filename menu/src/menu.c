/*
* menu_new.c
*
*  Created on: 17.03.2015
*      Author: Richard
*/


/*
* blabla.h
*
*  Created on: 19.03.2015
*      Author: Richard
*/

#include <menu/menu.h>

#define BUTTON_PRESS_LONG 'l'
#define BUTTON_PRESS_SHORT 'k'
#define BUTTON_PRESS_FREE 'f'

#define UP_KEY 'w'
#define DOWN_KEY 's'
#define RIGHT_KEY 'd'
#define LEFT_KEY 'a'

int16_t value[8];

MENU_ENTRY *actuall_entry = &main_entry;
uint16_t menu_display_selector = MENU_MAIN_ENTRY;

uint8_t menu_display()
{
	if (menu_display_selector == MENU_NO_FUNC_ENTRY) {
		if (actuall_entry->previous != 0) {
			lcd_create_view(actuall_entry->previous->text, 1, 0, 0, 0);
		}
		else {
			lcd_create_view(actuall_entry->parent->text, 0, 0, 0, 0);
		}
		//lcd_create_view("~", 0, 1, 0, 0);
		lcd_create_view(actuall_entry->text, 1, 1, 0, 0);
		if (actuall_entry->next != 0) {
			lcd_create_view(actuall_entry->next->text, 1, 2, 0, 0);
		}
		lcd_create_view("~", 0, 1, 0, 1);
	}

	return 0;
}

uint8_t menu_function(uint8_t *encoder_left_button, int8_t *encoder_left_count, uint8_t *encoder_right_button, int8_t *encoder_right_count)
{
	switch(actuall_entry->entry_num) {
	case RADIO_RDS_VIEW_ENTRY:
	case RADIO_RSQ_VIEW_ENTRY:
	case RADIO_PIPTY_VIEW_ENTRY:
		return radio_display_view(actuall_entry->entry_num);
	case MENU_FREQ_ENTRY:
		return menu_scroll_settings(encoder_right_button, encoder_right_count);
	case MENU_VOL_ENTRY:
		return menu_scroll_settings(encoder_right_button, encoder_right_count);
	case MENU_CONT_ENTRY:
		return menu_scroll_settings(encoder_right_button, encoder_right_count);
	case MENU_BRIG_ENTRY:
		return menu_scroll_settings(encoder_right_button, encoder_right_count);
	case MENU_MAIN_ENTRY:
		return radio_main(encoder_left_button, encoder_left_count, encoder_right_button, encoder_right_count);
	}
	//*encoder_right_button = BUTTON_FREE;
	return 0;
}

uint8_t menu_scroll_settings(uint8_t *encoder_button, int8_t *encoder_count)
{
	static int8_t value = 0;
	if(value == 0) {
		value = 23; //TODO load value from flash
	}
	if(*encoder_count > 0) {
		value++;
	} else if(*encoder_count < 0) {
		value--;
	}
	if(value > 100) {
		value = 100;
	} else if (value < 0) {
		value = 0;
	}
	*encoder_count = 0;
	menu_scroll((value * 60) / 100);
	if(*encoder_button == BUTTON_PRESS_SHORT) {
		//TODO store value to flash
		*encoder_button = BUTTON_FREE;
		value = 0;
		return 0xFD;
	}
	return 0;
}

uint8_t menu_scroll(uint8_t value)
{
	uint8_t i = 0;
	char string[2];
	value = (value * 60) / 100;

	lcd_create_view("-", 0, 2, 0, 0);
	for(i = 1; i <= value / 5 && i <= 12; i++) {
		lcd_create_view("\5", 1 + i, 2, 0, 0);
	}
	if(i <= 12) {
		string[0] = value % 5;
		lcd_create_view(string, 1 + i, 2, 0, 0);
	}
	i++;
	for(i = i; i <= 12; i++) {
		lcd_create_view("\0", 1 + i, 2, 0, 0);
	}
	lcd_create_view(" +", 14, 2, 0, 0);

	return 0;
}

uint8_t menu_handler(uint8_t *encoder_left_button, int8_t *encoder_left_count, uint8_t *encoder_right_button, int8_t *encoder_right_count)
{
	if (menu_display_selector == 0) {
		*encoder_left_count = 0;
		*encoder_left_button = BUTTON_PRESS_FREE;
		if (*encoder_right_button == BUTTON_PRESS_LONG) {
			if (actuall_entry->child_long != 0) {
				actuall_entry = actuall_entry->child_long;
			}
			if (actuall_entry->entry_num != 0) {
				menu_display_selector = actuall_entry->entry_num;
				menu_function(0, 0, 0, 0);
				return 0;
			}
			*encoder_right_button = BUTTON_PRESS_FREE;
		}
		if (*encoder_right_button == BUTTON_PRESS_SHORT) {
			if (actuall_entry->child_short != 0) {
				actuall_entry = actuall_entry->child_short;
			}
			if (actuall_entry->entry_num != 0) {
				menu_display_selector = actuall_entry->entry_num;
				*encoder_right_button = BUTTON_PRESS_FREE;
				menu_function(0, 0, 0, 0);
				return 0;
			}
			*encoder_right_button = BUTTON_PRESS_FREE;
		}
		if (*encoder_right_count != 0) {
			if(*encoder_right_count > 0) {
				if (actuall_entry->next != 0) {
					actuall_entry = actuall_entry->next;
				}
			}
			if (*encoder_right_count < 0) {
				if (actuall_entry->previous != 0) {
					actuall_entry = actuall_entry->previous;
				}
			}
			*encoder_right_count = 0;
		}
		menu_display();
	}else {
		uint8_t ret = menu_function(encoder_left_button, encoder_left_count, encoder_right_button, encoder_right_count);
		if (ret  == 0xFF) {		//Menu down wards
			actuall_entry = actuall_entry->child_short;
			menu_display_selector = 0;
			menu_display();
		} else if (ret == 0xFE) {
			actuall_entry = actuall_entry->child_long;
			menu_display_selector = 0;
			menu_display();
		} else if (ret == 0xFD) {
			menu_display_selector = 0;
			menu_display();
		}
	}
	return 0;
}
