#ifndef _MENU_H_
#define _MENU_H_

#include <stdint.h>
#include <driver/driver.h>

//----------------------------------------------------------------------------------------
//
/// \brief Definition of some important values
//
//----------------------------------------------------------------------------------------

typedef struct menu_stc {
	uint8_t x;
	uint8_t y;
	uint8_t old_x;
	uint8_t old_y;
} MENU_STC;

typedef uint8_t (*MENU_FUNC_PTR)(ENCODER *, ENCODER *, MENU_STC *);

typedef const struct menu_entry {
	const struct menu_entry *child;
	const struct menu_entry *parent;
	const struct menu_entry *next;
	const struct menu_entry *previous;
	const char *text;
	const MENU_FUNC_PTR func;
} MENU_ENTRY;

#include <menu/menu_struct.h>

#define LONG_INTO_MENU 0xFF
#define SHORT_INTO_MENU 0xFE
#define SHORT_UP_TO_CHILD 0xFD
#define SHORT_UP_TO_PARENT 0xFC
#define STAY_ON_MENU_POINT 0x00

enum MENU_ENTRY_NUM {
	MENU_FIRST_ENTRY = 0,
	MENU_SECOND_ENTRY,
	MENU_THIRD_ENTRY,
	MENU_FOURTH_ENTRY,
	MENU_FIFTH_ENTRY,
	MENU_SIXTH_ENTRY,
	MENU_SEVENTH_ENTRY,
};

//----------------------------------------------------------------------------------------
//
/// \brief Definition of functions for
///	-the menu handler
///	-scrolling through the menu
///	-main program of the menu
///	-scroll through the settings
//
//----------------------------------------------------------------------------------------

uint8_t menu_handler(ENCODER *encoder_left, ENCODER *encoder_right);
uint8_t menu_scroll(uint8_t value);
uint8_t menu_main(ENCODER *encoder);
uint8_t menu_scroll_settings(uint8_t value);
//uint8_t menu_function(uint8_t *encoder_left_button, int8_t *encoder_left_count, uint8_t *encoder_right_button, int8_t *encoder_right_count);

#endif /* _MENU_H_ */
