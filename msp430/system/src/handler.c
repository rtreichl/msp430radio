/*
 * handler.c
 *
 *  Created on: 05.02.2015
 *      Author: Richard
 */


/*
 * The handler calls functions time and interrupt based for the interrupts is a new interrupt handler needed!!!
 * The interrupt handler should reakt on different interrupts and look up in a table where the to calling function is!!!
 * Problem which is to solve is how to get an own interrupt handler on msp430 architectur.
 * For a good performance, interrupt function which is called, should be shortest as possible.
 * As next step there can be a function, that will countinouse the callculation in a normal operate state.
 * The funciton call after a interrupt servece the handler.
 * A further problem is the msp430 has a very low ram, so if many functions saving many variables during calculation.
 * Better implemention would be a not a multitheread system, only a single thread system and minimize waitings.
 */

#include <system/handler.h>

//----------------------------------------------------------------------------------------
//
/// \brief Handler for menu, RDS, time, the encoders and rsq
//
/// Steps in the while loop include:\n
///	(1)Calculate the new encoder values\n
///	(2)Update display\n
///	(3)Actualize menu\n
///	(4)Actualize encoders\n
///	(5)Update time\n
///	(6)Update rsq\n
///	(7)Update RDS
//
/// \param	void
//
/// \retval	void
//
/// \remarks
//
//----------------------------------------------------------------------------------------

void handler (void)
{
	ENCODER encoder_left, encoder_right;
	while(1) {
		encoder_1_update(&encoder_left);
		encoder_2_update(&encoder_right);
		if(*encoder_left.button != BUTTON_FREE || encoder_left.count != 0 || *encoder_right.button != BUTTON_FREE || encoder_right.count != 0 || timer_count[RADIO_TIMER_DISPLAY] >= RADIO_COUNT_DISPLAY) {
			if(timer_count[RADIO_TIMER_DISPLAY] >= RADIO_COUNT_DISPLAY) {
				radio.status.scroll_text++;
				if(radio.status.scroll_text == 78) {
					radio.status.scroll_text = 0;
				}
			}
			timer_count[RADIO_TIMER_DISPLAY] = 0;
			menu_handler(&encoder_left, &encoder_right);
		}
		if(timer_count[RADIO_TIMER_ENCODER] >= RADIO_COUNT_ENCODER) {
			timer_count[RADIO_TIMER_ENCODER] -= RADIO_COUNT_ENCODER;
			encoder_interrupt2();
		}
		if(timer_count[RADIO_TIMER_MINUTE] >= RADIO_COUNT_MINUTE) {
			timer_count[RADIO_TIMER_MINUTE] -= RADIO_COUNT_MINUTE;
			time_update();
		}
		if(timer_count[RADIO_TIMER_RSQ] >= RADIO_COUNT_RSQ) {
			rsq_update(&radio);
			timer_count[RADIO_TIMER_RSQ] -= RADIO_COUNT_RSQ;
		}
		if(timer_count[RADIO_TIMER_RDS] >= RADIO_COUNT_RDS) { //TODO Rework to interrupt base system
			rds_update(&radio);
			timer_count[RADIO_TIMER_RDS] -= RADIO_COUNT_RDS;
		}
	}
}
