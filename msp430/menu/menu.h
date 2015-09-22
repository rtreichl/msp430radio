///////////////////////////////////////////////////////////////////////////////////////////
///
/// \file menu.h
///
/// \brief menu.h includes all necessary functions macros and structs for the radio data\n
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

///////////////////////////////////////////////////////////////////////////////////////////
///
/// \defgroup Menu Menu, Structs and Functions
/// \brief In here the are structs declared for several RDS stuff(See http://www.g.laroche.
///	free.fr/english/rds/groupes/listeGroupesRDS.htm for more information). Also there are
///	functions declared to update RDS and to get the	current time out of RDS.\n\n
///
/// The group RDS allows to identify the transmitted type of data. There are 16 groups\n
/// which can each decline in 2 versions A or B.\n
/// The group type and the version (A or B) are transmitted in the second block (block B)}n
/// of RDS group. The bits A0 -> A3 indicate the group type (0= group 0, .... F = group\n
/// 15) and the bit B0 indicate the version (0 = A version ).
///
///	@{
///
///////////////////////////////////////////////////////////////////////////////////////////

#ifndef _MENU_H_
#define _MENU_H_

#include <stdint.h>
#include <driver/driver.h>

#define LONG_INTO_MENU 0xFF
#define SHORT_INTO_MENU 0xFE
#define SHORT_UP_TO_CHILD 0xFD
#define SHORT_UP_TO_PARENT 0xFC
#define STAY_ON_MENU_POINT 0x00
#define MENU_ENCODER_CHANGED 1
#define MENU_ENCODER_NOP 0
#define TRUE 1

enum MENU_ENTRY_NUM {
	MENU_FIRST_ENTRY = 0,
	MENU_SECOND_ENTRY,
	MENU_THIRD_ENTRY,
	MENU_FOURTH_ENTRY,
	MENU_FIFTH_ENTRY,
	MENU_SIXTH_ENTRY,
	MENU_SEVENTH_ENTRY,
};

///////////////////////////////////////////////////////////////////////////////////////////
///
/// \struct menu_stc
/// \brief Definition of some important values
///
/// \var menu_stc::x
/// \brief test123
///
/// \var menu_stc::y
/// \brief
///
/// \var menu_stc::old_x
/// \brief
///
/// \var menu_stc::old_y
/// \brief
///
///////////////////////////////////////////////////////////////////////////////////////////

typedef struct menu_stc {
	uint8_t x;
	uint8_t y;
	uint8_t old_x;
	uint8_t old_y;
} MENU_STC;

///////////////////////////////////////////////////////////////////////////////////////////
///
/// \typedef MENU_FUNC_PTR
/// \brief Definition of some important values
///
///////////////////////////////////////////////////////////////////////////////////////////

typedef uint8_t (*MENU_FUNC_PTR)(ENCODER *, ENCODER *, MENU_STC *);

///////////////////////////////////////////////////////////////////////////////////////////
///
/// \struct menu_entry
/// \brief Definition of some important values
///
/// \var menu_entry::child
/// \brief test123
///
/// \var menu_entry::parent
/// \brief
///
/// \var menu_entry::next
/// \brief
///
/// \var menu_entry::previous
/// \brief
///
/// \var menu_entry::text
/// \brief
///
/// \var menu_entry::func
/// \brief
///
///////////////////////////////////////////////////////////////////////////////////////////

typedef const struct menu_entry {
	const struct menu_entry *child;
	const struct menu_entry *parent;
	const struct menu_entry *next;
	const struct menu_entry *previous;
	const char *text;
	const MENU_FUNC_PTR func;
} MENU_ENTRY;

#include <menu/menu_struct.h>


//----------------------------------------------------------------------------------------
//
/// \brief Definition of functions for
///	-the menu handler
///	-scrolling through the menu
///	-main program of the menu
///	-scroll through the settings
//
//----------------------------------------------------------------------------------------

uint8_t menu display(ENCODER * encoder_left, ENCODER *encoder_right);

///////////////////////////////////////////////////////////////////////////////////////////
///
/// \param [in] encoder_left 	 left encoder data
/// \param [in] encoder_right	 right encoder data
///
/// \retval uint8_t
///
///////////////////////////////////////////////////////////////////////////////////////////

uint8_t menu_handler(ENCODER *encoder_left, ENCODER *encoder_right);

///////////////////////////////////////////////////////////////////////////////////////////
///
/// \param [in] value	position to be scrolled toa
///
/// \retval uint8_t
///
///////////////////////////////////////////////////////////////////////////////////////////

uint8_t menu_scroll(uint8_t value);

///////////////////////////////////////////////////////////////////////////////////////////
///
/// \param [in] encoder	position to be scrolled toa
///
/// \retval uint8_t
///
///////////////////////////////////////////////////////////////////////////////////////////

uint8_t menu_scroll_settings(uint8_t value);

///////////////////////////////////////////////////////////////////////////////////////////
///
/// \param [in] encoder			Encoder which is applied for this setting
/// \param [in] control			Value which should be scrolled
/// \param [in] bytes			Size of control
/// \param [in] upper_bound		Upper bound for control
/// \param [in] lower_bound		Lower bound for control
/// \param [in] step			Step size for control
/// \param [in] overflow		1 = overflow for control on bounds\n 0 = control stuck at bounds
///
/// \retval uint8_t
///
///////////////////////////////////////////////////////////////////////////////////////////

uint8_t menu_encoder_range(ENCODER *encoder, void *control, uint8_t bytes, uint16_t upper_bound, uint16_t lower_bound, uint8_t step, uint8_t overflow);

///////////////////////////////////////////////////////////////////////////////////////////
///
///	@}
///
///////////////////////////////////////////////////////////////////////////////////////////

#endif /* _MENU_H_ */
