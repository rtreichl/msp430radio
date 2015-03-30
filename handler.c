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


void handler (void)
{
	while(1) {
		//TODO check for rds interrupt and go on further functions
		//TODO call all half second or on specified input aciton menu function
		//TODO do all 0.1 messuarments on a valid channel
		//TODO implement timers and eventtimers
		//TODO polling encoder switches every 10 up to 20 milli seconds
	}
}
