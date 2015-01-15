/*
 * MSP430G2553_Clock_Timer.c
 *
 *  Created on: 12.07.2013
 *      Author: Alexander Raskopf
 */

#include <msp430.h>
#include "MSP430G2553_Clock_Timer.h"


void Freq1MHZ_INIT(void)
{
if((CALDCO_1MHZ!=0xFF)&&(CALBC1_1MHZ!=0xFF))
{
DCOCTL=CALDCO_1MHZ;
BCSCTL1=CALBC1_1MHZ;
}
}

void Freq8MHZ_INIT(void)
{
if((CALDCO_8MHZ!=0xFF)&&(CALBC1_8MHZ!=0xFF))
{
DCOCTL=CALDCO_8MHZ;
BCSCTL1=CALBC1_8MHZ;
//DCOCTL=0x60;//CAL_DCO_8MHZ;
//BCSCTL1=0x0D;//CAL_BC1_8MHZ;
}
}

void Freq12MHZ_INIT(void)
{
if((CALDCO_12MHZ!=0xFF)&&(CALBC1_12MHZ!=0xFF))
{
DCOCTL=CALDCO_12MHZ;
BCSCTL1=CALBC1_12MHZ;
}
}

void Freq16MHZ_INIT(void)
{
if((CALDCO_16MHZ!=0xFF)&&(CALBC1_16MHZ!=0xFF))
{
DCOCTL=CALDCO_16MHZ;
BCSCTL1=CALBC1_16MHZ;
}
}


void Clock_INIT(void)
{
	BCSCTL2 = SELM_0 + DIVM_3 + DIVS_2;

	if(CALBC1_16MHZ != 0xFF)
	{
    //__delay_cycles(100000);
    DCOCTL = 0x00;
    BCSCTL1 = CALBC1_16MHZ;
    DCOCTL = CALDCO_16MHZ;
	}
}

//New Clock Test for 4MHz Clock using DCO

void Clock_INIT1(void)
{
	Freq8MHZ_INIT();

	BCSCTL2 = SELM_0 + DIVM_1 + DIVS_1;
}

void Clock_INIT2(void)
{
	Freq8MHZ_INIT();

	//Freq16MHZ_INIT();

	BCSCTL2 |= SELM_0 + DIVM_1 + DIVS_1;
	//BCSCTL2 = SELM_0 + DIVM_2 + DIVS_2;
}
