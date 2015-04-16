/*
 * Timer.h
 *
 *  Created on: 17.12.2013
 *      Author: Richi
 */

#ifndef TIMER_H_
#define TIMER_H_

#include <msp430.h>
#include <stdint.h>
#include <settings/radio_configuration.h>

#define TIMER_COUNTS RADIO_TIMER_USED

extern volatile uint16_t timer_count[];

void basic_clock_init(void);
void timer_init();
void _delay_ms(uint16_t delay);
void _delay_ten_us(uint16_t delay);

#endif /* TIMER_H_ */
