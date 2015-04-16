/*
 * main.c
 *
 *  Created on: 18.06.2013
 *      Author: Alexander Raskopf
 *      Status: Nach TEST
 *
 *   Kommentar: Tasterabfrage verbessern
 *   			schleifen durch Timer ersetzen
 */

#include <system/handler.h>

int main (void)
{
	WDTCTL = WDTPW + WDTHOLD;
	radio_init();
	handler();
}

//Not in used interrupt vectors

#pragma vector=WDT_VECTOR
__interrupt void wdt(void){}
#pragma vector=NMI_VECTOR
__interrupt void nmi(void){}
#pragma vector=TIMER1_A1_VECTOR
__interrupt void tim1(void){}
#pragma vector=ADC10_VECTOR
__interrupt void adc(void){}
#pragma vector=COMPARATORA_VECTOR
__interrupt void comp(void){}
