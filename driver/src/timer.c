/*
 * Timer.c
 *
 *  Created on: 17.12.2013
 *      Author: Richi
 */

#include <driver/timer.h>

volatile uint16_t timer_count[TIMER_COUNTS];
volatile uint16_t timer_delay;


/*
#define MAX_TIMER 10

uint8_t id;

struct TIME_HANDLER {
	uint16_t ms;
	uint8_t done;
} time_handler[MAX_TIMER];
*/

void basic_clock_init(void)
{
	if (CALBC1_16MHZ==0xFF)	{					// If calibration constant erased
		  while(1);                               // do not load, trap CPU!!
	}
	DCOCTL = 0;                               // Select lowest DCOx and MODx settings
	BCSCTL1 = RADIO_CALBC1;                   // Set range
	DCOCTL = RADIO_CALDCO;                    // Set DCO step + modulation
	BCSCTL1 |= DIVA_2;						  // ACLK dived by 8
	BCSCTL2 = SELM_0 | DIVM_0 | DIVS_3;
	BCSCTL3 = LFXT1S_0 | XCAP_0;

	FCTL2 = FWKEY + FSSEL_2 + 0x05;             // MCLK/3 for Flash Timing Generator
}
/*
uint8_t create_timer(uint8_t mode, uint8_t s, uint8_t ms)
{
	if(id < MAX_TIMER) {
		if(us >= 100 && us < 1000) {
			if(ms < 1000) {
				if(s < 3) {
					time_handler[id].ms = 1000 * s + ms;
					time_handler[id].mode = mode;
					time_handler[id].done = 0;
					id++;
				}
				else {
					return 0xFE;
				}
			}
			else {
				return 0xFD;
			}
		}
		else {
			return 0xFC;
		}
	}
	else {
		return 0xFB;
	}
	return id - 1;
}

uint8_t delete_timer(uint8_t id)
{
	time_handler[id].us = 0;
	time_handler[id].mode = 0;
	time_handler[id].done = 1;
	id--;
	return 0x00;
}

uint8_t get_state_timer(uint8_t id)
{
	if(time_handler[id].mode != 0) {
		return timer_handler[id].done;
	}
	else {
		return 0xFF;
	}
}
*/


void timer_init()
{
	TA0CCTL0 |= CCIE;			//enable interrupt on compare
	TA0CTL = TASSEL_2 | ID_2;	//SMCLK 2 MHz dived by 8 are 500 kHz = 2 us per count
	TA0CCR0 = 500;				//500 counts are 1 ms for interrupt
	TA0CTL |= MC_1;				//up-mode counts up to TACCR0 and restart*/

	//TA1CCTL0 |= CCIE;			//enable interrupt on compare
	TA1CTL = TASSEL_2;			//SMCLK 2 MHz = 500ns per count
	TA1CCR0 = 20;				//500 counts are 10 us for interrupt
	TA1CTL |= MC_1;				//up-mode counts up to TACCR0 and restart

}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A0(void)
{
	uint8_t i;
	for(i = TIMER_COUNTS; i-- > 0;) {
		timer_count[i]++;
	}
}

#pragma vector=TIMER1_A0_VECTOR
__interrupt void Timer_A1(void)
{
	timer_delay++;
}

void _delay_ms(uint16_t delay)
{
	delay *= 100;
	_delay_ten_us(delay);
}

void _delay_ten_us(uint16_t delay)
{
	timer_delay = 0;
	TA1CCTL0 |= CCIE;
	while(timer_delay < delay);
	TA1CCTL0 &= ~CCIE;
}


/*TODO Implement various timers and a timer handler for event timers and normal timers to calculate runtimes
 * for function on main handler. Timer initialization should be include setup base clock on given configuration
 * and also a function that gives back the actuall working frequency for other functions, who are needing a frequency
 * for intialization. Timer initialization will be started before all other functions, so it´s placed in to system_pre_init();
 */
