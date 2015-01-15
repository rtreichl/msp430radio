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
