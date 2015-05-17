/*
* menu_new.c
*
*  Created on: 17.03.2015
*      Author: Richard
*/

#include <menu/menu.h>

//----------------------------------------------------------------------------------------
//
/// \brief Definition of some important values
//
//----------------------------------------------------------------------------------------

MENU_ENTRY *actuall_entry = &main_long_entry;
MENU_FUNC_PTR actuall_func = &radio_main;

//----------------------------------------------------------------------------------------
//
/// \brief Display the current menu
//
/// \param
//
/// \retval uint8_t
//
/// \remarks
//
//----------------------------------------------------------------------------------------

uint8_t menu_display()
{
	if (actuall_func == MENU_NO_FUNC_ENTRY) {
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

/*uint8_t menu_function(uint8_t *encoder_left_button, int8_t *encoder_left_count, uint8_t *encoder_right_button, int8_t *encoder_right_count)
{
	if(menu_display_selector == MENU_MAIN_ENTRY) {
		return radio_main(encoder_left_button, encoder_left_count, encoder_right_button, encoder_right_count);
	}
	else {
		return radio_settings(encoder_right_button, encoder_right_count, actuall_entry->entry_num);
	}
}*/

//----------------------------------------------------------------------------------------
//
/// \brief Scroll through the settings
//
/// \param	<value>	[in]	scroll value
//
/// \retval uint8_t
//
/// \remarks
//
//----------------------------------------------------------------------------------------

uint8_t menu_scroll_settings(uint8_t value)
{
	char tmp_string[4];
	lcd_create_view(actuall_entry->text, 0, 0, 0, 0);
	string_int_to_array(tmp_string, value, 3, 10);
	lcd_create_view(tmp_string, 0, 1, 0, 0);
	menu_scroll(value);
	lcd_create_view("%", 3, 1, 0, 1);
	return 0;
}

//----------------------------------------------------------------------------------------
//
/// \brief Scroll to given position
//
/// \param	<value>	[in]	position to be scrolled to
//
/// \retval uint8_t
//
/// \remarks
//
//----------------------------------------------------------------------------------------

uint8_t menu_scroll(uint8_t value)
{
	uint8_t i = 0;
	char string[2];
	value = (value * 60) / 100;

	lcd_create_view("- ", 0, 2, 0, 0);
	for(i = 1; i <= value / 5 && i <= 12; i++) {
		lcd_create_view("\5", 1 + i, 2, 0, 0);
	}
	if(i <= 12) {
		string[0] = value % 5;
		lcd_create_view(string, 1 + i, 2, 0, 0);
	}
	i++;
	for(; i <= 12; i++) {
		lcd_create_view("\0", 1 + i, 2, 0, 0);
	}
	lcd_create_view(" +", 14, 2, 0, 0);

	return 0;
}

#define MENU_ENCODER_CHANGED 1
#define MENU_ENCODER_NOP 0

uint8_t menu_encoder_range(int8_t *encoder_right_count, uint8_t *controll, uint8_t upper_bound, uint8_t lower_bound, uint8_t step)
{
	if(*encoder_right_count != 0) {
		if(*encoder_right_count > 0 && *controll < upper_bound) {
			*controll += step;
		}
		else if(*encoder_right_count < 0 && *controll > lower_bound) {
			*controll -= step;
		}
		return MENU_ENCODER_CHANGED;
	}
	return MENU_ENCODER_NOP;
}

//----------------------------------------------------------------------------------------
//
/// \brief Handler of the menu, entry to the sub menus and the parent lvl of the menus
//
/// \param	<encoder_left_button>	[in]	State of the left button
/// \param	<encoder_left_count>	[in]	Count of the left encoder
/// \param	<encoder_right_button>	[in]	State of the right button
/// \param	<encoder_right_count>	[in]	Count of the right encoder
//
/// \retval uint8_t
//
/// \remarks
//
//----------------------------------------------------------------------------------------

uint8_t menu_handler(uint8_t *encoder_left_button, int8_t *encoder_left_count, uint8_t *encoder_right_button, int8_t *encoder_right_count)
{
	if (actuall_func == MENU_NO_FUNC_ENTRY) {
		*encoder_left_count = 0;

		if (*encoder_right_button == BUTTON_SHORT) {
			if (actuall_entry->child != 0) {
				actuall_entry = actuall_entry->child;
				if(actuall_entry->child == menu_long_entry || actuall_entry->child == menu_short_entry) {
					actuall_func = actuall_entry->func;
					*encoder_right_button = BUTTON_FREE;
					actuall_func(encoder_left_button, encoder_left_count, encoder_right_button, encoder_right_count, actuall_entry->entry_num);
					return 0;
				}
			}
			else if (actuall_entry->func != 0) {
				actuall_func = actuall_entry->func;
				*encoder_right_button = BUTTON_FREE;
				menu_handler(encoder_left_button, encoder_left_count, encoder_right_button, encoder_right_count);
				return 0;
			}
			*encoder_right_button = BUTTON_FREE;
		}

		if(*encoder_left_button == BUTTON_SHORT) {
			if(actuall_entry->parent != 0) {
				actuall_entry = actuall_entry->parent;
				if(actuall_entry->child == menu_long_entry || actuall_entry->child == menu_short_entry) {
					actuall_func = actuall_entry->func;
					*encoder_left_button = BUTTON_FREE;
					actuall_func(encoder_left_button, encoder_left_count, encoder_right_button, encoder_right_count, actuall_entry->entry_num);
					return 0;
				}
			}
			*encoder_left_button = BUTTON_FREE;
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
		switch(actuall_func(encoder_left_button, encoder_left_count, encoder_right_button, encoder_right_count, actuall_entry->entry_num)) {
		case LONG_INTO_MENU:
			actuall_entry = menu_long_entry;
			actuall_func = MENU_NO_FUNC_ENTRY;
			menu_display();
			break;
		case SHORT_INTO_MENU:
			actuall_entry = menu_short_entry;
			actuall_func = MENU_NO_FUNC_ENTRY;
			menu_display();
			break;
		case SHORT_UP_TO_CHILD:
			actuall_func = MENU_NO_FUNC_ENTRY;
			menu_display();
			break;
		case SHORT_UP_TO_PARENT:
			actuall_func = MENU_NO_FUNC_ENTRY;
			actuall_entry = actuall_entry->parent;
			if(actuall_entry->child == menu_long_entry || actuall_entry->child == menu_short_entry) {
				actuall_func = actuall_entry->func;
				actuall_func(encoder_left_button, encoder_left_count, encoder_right_button, encoder_right_count, actuall_entry->entry_num);
				//timer_count[4] += 500;
				//menu_function(0, 0, 0, 0);
				return 0;
			}
			menu_display();
			break;
		}
	}
	return 0;
}
