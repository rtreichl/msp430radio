/*
 * handler.c
 *
 *  Created on: 05.02.2015
 *      Author: Richard
 */


/*
 * The handler calls frunctions time and interrupt based for the interrupts is a new interrupt handler needed!!!
 * The interrupt handler should reakt on different interrupts and look up in a table where the to calling function is!!!
 * Problem which is to solve is how to get an own interrupt handler on msp430 architectur.
 * For a good performance, interrupt function which is called, should be shortest as possible.
 * As next step there can be a function, that will countinouse the callculation in a normal operate state.
 * The funciton call after a interrupt servece the handler.
 * A further problem is the msp430 has a very low ram, so if many functions saving many variables during calculation.
 * Better implemention would be a not a multitheread system, only a single thread system and minimize waitings.
 */
#include <system/handler.h>

#define	TIME_SECOND	1000
#define ENCODER_TAST_REFRESH	10

void handler (void)
{
	//TODO collect all function for timer based and calling functions
	while(1) {
		if(timer_count[0] >= TIME_SECOND) {
			timer_count[0] -= TIME_SECOND;
			//call time_update();
		}
		if(timer_count[1] >= ENCODER_TAST_REFRESH) {
			timer_count[1] -= ENCODER_TAST_REFRESH;
			//call encoder_interrupt2();
		}
		/*if(encoder_2 != 0 || encoder_1 != 0 || encoder_1_button != 'f' || encoder_2_button != 'f') {
			call menu_handler
		}
		 */
		//TODO check for rds interrupt and go on further functions
		//TODO call all half second or on specified input aciton menu function
		//TODO do all 0.1 messuarments on a valid channel
		//TODO implement timers and eventtimers
		//TODO polling encoder switches every 10 up to 20 milli seconds
	}
}
