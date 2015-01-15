/*
 * MSP430G2553_GPIO.c
 *
 *  Created on: 18.07.2013
 *      Author: Alexander Raskopf
 */
#include <msp430g2553.h>
#include "MSP430G2553_GPIO.h"
#include "Timer.h"

//char falling_edge = 0;

void GPIO_INIT(void)
{
    //SEN
	P3DIR |=   BIT4;
	P3OUT &=~  BIT4;

	//GPO2/INT
	//P2DIR |= BIT4;

	// Clear Reset
	P2DIR |=  BIT3;
	P2OUT &=~  BIT3;

	_delay_ms(1);

	// Set Reset
	//P2DIR |=  BIT3;		// legt P2.1 Register als Ausgang fest
	P2OUT |=  BIT3;		// Ausgang auf low gesetzt

	_delay_ms(2);
	P2REN |= BIT4;
	P2OUT |= BIT4;
	//P2IES |= BIT4;
	//P2IFG &= ~BIT4;
	//P2IE |= BIT4;
	//_EINT();
	//P3DIR |=   BIT5;
	//P3OUT |=   BIT5;

	//SHOTDOWN
	//P2DIR |=  BIT5;
	//P2OUT &=~ BIT5;		// Ausgang auf high gesetzt

	//ROTE LED
    //P1DIR |=  BIT0;
	//P1OUT &=~ BIT0;

	//P3DIR |= BIT0;
	//P3OUT &=~BIT0;
}
