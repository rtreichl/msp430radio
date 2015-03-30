/*
 * Timer.c
 *
 *  Created on: 17.12.2013
 *      Author: Richi
 */

#include <msp430.h>
#include "Timer.h"

volatile unsigned int count;

void _delay_ms(int ms)
{
	unsigned int kaka = 0;
	do
	{
		_delay_cycles(8000);
	}while(++kaka != ms);

}

void _delay_us(int us)
{
	 unsigned int kaka = 0;
	do
	{
		_delay_cycles(8);
	}while(++kaka != us);
}


/*TODO Implement various timers and a timer handler for event timers and normal timers to calculate runtimes
 * for function on main handler. Timer initialization should be include setup base clock on given configuration
 * and also a function that gives back the actuall working frequency for other functions, who are needing a frequency
 * for intialization. Timer initialization will be started before all other functions, so it´s placed in to system_pre_init();
 */
