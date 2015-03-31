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

#define I2C_LCD_BAUDRATE			40



#include <msp430.h>
#include <msp430g2553.h>
#include <intrinsics.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "driver/i2c.h"
#include <driver/si4735.h>
#include <driver/pca9530.h>
//#include "AudioSwitch.h"
#include <driver/tpa2016d2.h>
#include <driver/lcd.h>
#include <driver/encoder.h>
#include <driver/timer.h>
#include "Menu.h"
#include <menu/menu.h>
#include <system/radio.h>
#include <driver/timer.h>

#define ENCODER_TAST_REFRESH	10
#define	TIME_SECOND	50

volatile unsigned char sekunde = 0;
volatile unsigned char posrt = 0;
volatile unsigned char sec = 240;

extern volatile unsigned char encoder_1_button, encoder_2_button, sekunde, sec , posrt;

int main (void)
{
	WDTCTL = WDTPW + WDTHOLD;
	radio_init();
	int8_t en_counter2;
	time_set(18,29,27,2,16,0);

	while(1)
	{
		//menu();
		en_counter2 +=  Encoder_2_get_count();
		menu_handler(&encoder_2_button, &en_counter2);
		if(timer_count[1] >= ENCODER_TAST_REFRESH) {
		timer_count[1] -= ENCODER_TAST_REFRESH;
		encoder_interrupt2();
		}
		if(timer_count[3] >= TIME_SECOND) {
		timer_count[3] -= TIME_SECOND;
		time_date_update();
		}
		//encoder_2_button = 'f';
		//en_counter2 = 0;
	}

}

#pragma vector=WDT_VECTOR
__interrupt void watchdog_timer(void)
{
	if((sekunde++)%2 == 0)		//wenn eine Halbe sekunde vergangen ist schiebe den Text um eins weiter
		if(posrt++ == 78)		//wenn der Text durch geshiftet ist fange von vorne wieder an
			posrt = 0;
	if(++sec >= 240)			//wenn eine Minute vorbei ist dann setze den zähler zurück und erhöhe die Minute um eins
	{
		sec = 0;
		//time_date(0, 0, 0, 0, 0, 0, 1, 0, 0);
	}
}

//Nicht benutzte Interrupt Vectoren

#pragma vector=NMI_VECTOR
__interrupt void nmi(void){}
#pragma vector=TIMER1_A1_VECTOR
__interrupt void tim1(void){}
//#pragma vector=USCIAB0RX_VECTOR
//__interrupt void us_rx(void){}
//#pragma vector=USCIAB0TX_VECTOR
//__interrupt void us_tx(void){}
#pragma vector=ADC10_VECTOR
__interrupt void adc(void){}
#pragma vector=COMPARATORA_VECTOR
__interrupt void comp(void){}
//#pragma vector=TIMER1_A0_VECTOR
//__interrupt void Timer_A(void){}
