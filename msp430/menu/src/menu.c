///////////////////////////////////////////////////////////////////////////////////////////
///
/// \file menu.c
///
/// \brief menu.c includes all necessary functions macros and structs for the radio data\n
/// service
///
/// \date 17.03.2015
///
/// \author Richard Treichl
///
/// \remark
///
/// \todo Finish documentation
///
/// \version	1.0
///
///////////////////////////////////////////////////////////////////////////////////////////

#include <menu/menu.h>
#include <radio/radio.h>
#include <libs/string.h>

///////////////////////////////////////////////////////////////////////////////////////////
///
/// \brief Definition of some important values
///
///////////////////////////////////////////////////////////////////////////////////////////

#define MENU_NO_FUNC_ENTRY 0

MENU_ENTRY *actuall_entry = &main_long_entry;
MENU_FUNC_PTR actuall_func = &radio_main;
MENU_STC menu = {0, 0, 0, 0};

///////////////////////////////////////////////////////////////////////////////////////////
///
/// \brief  Display the current menu
///
/// \remarks
///
/// \todot
///
///////////////////////////////////////////////////////////////////////////////////////////

uint8_t menu_display(ENCODER *encoder_left, ENCODER *encoder_right)
{
	if(actuall_entry->parent->func != 0 && actuall_entry->child == 0) {
		if(actuall_entry->parent->func != &radio_main) {
			actuall_entry->parent->func(encoder_left, encoder_right, &menu);
		}
	}
	if (actuall_func == MENU_NO_FUNC_ENTRY) {
		if (actuall_entry->previous != 0) {
			lcd_create_view(actuall_entry->previous->text, 1, 0, 0, 0);
		}
		else {
			lcd_create_view(actuall_entry->parent->text, 0, 0, 0, 0);
		}
		lcd_create_view(actuall_entry->text, 1, 1, 0, 0);
		if (actuall_entry->next != 0) {
			lcd_create_view(actuall_entry->next->text, 1, 2, 0, 0);
		}
		lcd_create_view(symbol_arrow_text, 0, 1, 0, 1);
	}

	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////
///
/// \brief Scroll through the settings
///
/// \remarks
///
/// \todot
///
///////////////////////////////////////////////////////////////////////////////////////////

uint8_t menu_scroll_settings(uint8_t value)
{
	char tmp_string[4];
	lcd_create_view(actuall_entry->text, 0, 0, 0, 0);
	itoa(value, tmp_string, 10, 3);
	lcd_create_view(tmp_string, 0, 1, 0, 0);
	menu_scroll(value);
	lcd_create_view(unit_percentage_text, 3, 1, 0, 1);
	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////
///
/// \brief Scroll to given position
///
/// \remarks
///
/// \todot
///
///////////////////////////////////////////////////////////////////////////////////////////

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

///////////////////////////////////////////////////////////////////////////////////////////
///
/// \brief Detected if the encoder hase changed. If encoder has changed than it will check
/// the boundary condition and execute necessary operation.
///
/// \remarks
///
/// \todo
///
///////////////////////////////////////////////////////////////////////////////////////////

uint8_t menu_encoder_range(ENCODER *encoder, void *controll, uint8_t bytes, uint16_t upper_bound, uint16_t lower_bound, uint8_t step, uint8_t overflow)
{
	uint16_t tmp_controll = 0;
	if(encoder->count != 0) {
		if(bytes > 1) {
			tmp_controll = *(uint16_t *)controll;
		}
		else {
			tmp_controll = *(uint8_t *)controll;
		}
		if(encoder->count > 0) {
			tmp_controll += step;
		}
		else if(encoder->count < 0) {
			tmp_controll -= step;
		}
		if(tmp_controll > upper_bound) {
			if(overflow == TRUE) {
				tmp_controll = lower_bound;
			}
			else {
				tmp_controll = upper_bound;
			}
		}
		else if(tmp_controll < lower_bound) {
			if(overflow == TRUE) {
				tmp_controll = upper_bound;
			}
			else {
				tmp_controll = lower_bound;
			}
		}
		if(bytes > 1) {
			*(uint16_t *)controll = tmp_controll;
		}
		else {
			*(uint8_t *)controll = tmp_controll;
		}
		encoder->count = 0;
		return MENU_ENCODER_CHANGED;
	}
	return MENU_ENCODER_NOP;
}

///////////////////////////////////////////////////////////////////////////////////////////
///
/// \brief Handler of the menu, entry to the sub menus and the parent lvl of the menus
///
/// \remarks
///
/// \todo
///
///////////////////////////////////////////////////////////////////////////////////////////

uint8_t menu_handler(ENCODER *encoder_left, ENCODER *encoder_right)
{
	if (actuall_func == MENU_NO_FUNC_ENTRY) {
		encoder_left->count = 0;

		if (*encoder_right->button == BUTTON_SHORT) {
			if (actuall_entry->child != 0) {
				if(actuall_entry->child == actuall_entry->parent) {
					menu.y = menu.old_y;
					menu.x--;
				}
				else {
					menu.old_y = menu.y;
					menu.x++;
					menu.y = 0;
				}
				actuall_entry = actuall_entry->child;
				if(actuall_entry->child == menu_long_entry || actuall_entry->child == menu_short_entry) {
					actuall_func = actuall_entry->func;
					*encoder_right->button = BUTTON_FREE;
					actuall_func(encoder_left, encoder_right, &menu);
					menu.x--;
					return 0;
				}
			}
			else if (actuall_entry->func != 0) {
				actuall_func = actuall_entry->func;
				*encoder_right->button = BUTTON_FREE;
				menu_handler(encoder_left, encoder_right);
				return 0;
			}
			*encoder_right->button = BUTTON_FREE;
		}

		if(*encoder_left->button == BUTTON_SHORT) {
			if(actuall_entry->parent != 0) {
				actuall_entry = actuall_entry->parent;
				menu.y = menu.old_y;
				menu.x--;
				if(actuall_entry->child == menu_long_entry || actuall_entry->child == menu_short_entry) {
					actuall_func = actuall_entry->func;
					*encoder_left->button = BUTTON_FREE;
					actuall_func(encoder_left, encoder_right, &menu);
					return 0;
				}
			}
			*encoder_left->button = BUTTON_FREE;
		}

		if (encoder_right->count != 0) {
			if(encoder_right->count > 0) {
				if (actuall_entry->next != 0) {
					menu.y++;
					actuall_entry = actuall_entry->next;
				}
			}
			if (encoder_right->count < 0) {
				if (actuall_entry->previous != 0) {
					menu.y--;
					actuall_entry = actuall_entry->previous;
				}
			}
			encoder_right->count = 0;
		}
		menu_display(encoder_left, encoder_right);
	}else {
		switch(actuall_func(encoder_left, encoder_right, &menu)) {
		case LONG_INTO_MENU:
			actuall_entry = menu_long_entry;
			actuall_func = MENU_NO_FUNC_ENTRY;
			menu_display(encoder_left, encoder_right);
			menu.x++;
			menu.y = 0;
			break;
		case SHORT_INTO_MENU:
			actuall_entry = menu_short_entry;
			actuall_func = MENU_NO_FUNC_ENTRY;
			menu_display(encoder_left, encoder_right);
			menu.x++;
			menu.y = 0;
			break;
		case SHORT_UP_TO_CHILD:
			actuall_func = MENU_NO_FUNC_ENTRY;
			menu_display(encoder_left, encoder_right);
			menu.x--;
			break;
		case SHORT_UP_TO_PARENT:
			actuall_func = MENU_NO_FUNC_ENTRY;
			actuall_entry = actuall_entry->parent;
			menu.y = menu.old_y;
			menu.x -= 2;
			if(actuall_entry->child == menu_long_entry || actuall_entry->child == menu_short_entry) {
				actuall_func = actuall_entry->func;
				actuall_func(encoder_left, encoder_right, &menu);
				return 0;
			}
			menu_display(encoder_left, encoder_right);
			break;
		}
	}
	return 0;
}
