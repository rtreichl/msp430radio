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

#include <msp430.h>
#include <intrinsics.h>
#include <stdint.h>
#include "driver/i2c.h"
#include <driver/si4735.h>
#include <driver/pca9530.h>
#include <driver/tpa2016d2.h>
#include <driver/lcd.h>
#include <driver/encoder.h>
#include <driver/timer.h>
#include <menu/menu.h>
#include <system/rsq.h>
#include <system/rds.h>
#include <radio/radio.h>
#include <driver/timer.h>

#define ENCODER_TAST_REFRESH	10
#define	TIME_MINUTE	60000
#define RSQ_UPDATE	1000
#define RDS_UPDATE	200
#define RADIO_TEXT_SCROLL 500

#define DISPLAY_REFRESH 500

int main (void)
{
	WDTCTL = WDTPW + WDTHOLD;
	uint8_t *encoder_left_button, *encoder_right_button;
	int8_t encoder_left_count = 0, encoder_right_count = 0;
	radio_init();
	time_set_time(18,29,27,2,16,0);

	while(1)
	{
		//menu();
		encoder_1_update(&encoder_left_count, &encoder_left_button);
		encoder_2_update(&encoder_right_count, &encoder_right_button);
		if(*encoder_left_button != BUTTON_FREE || encoder_left_count != 0 || *encoder_right_button != BUTTON_FREE || encoder_right_count != 0 || timer_count[4] >= DISPLAY_REFRESH) {
			menu_handler(encoder_left_button, &encoder_left_count, encoder_right_button, &encoder_right_count);
			timer_count[0] = 0;
			timer_count[4] = 0;
		}
		if(timer_count[1] >= ENCODER_TAST_REFRESH) {
			timer_count[1] -= ENCODER_TAST_REFRESH;
			encoder_interrupt2();
		}
		if(timer_count[3] >= TIME_MINUTE) {
			timer_count[3] -= TIME_MINUTE;
			time_update();
		}
		if(timer_count[6] >= RSQ_UPDATE) {
			rsq_update(&radio);
			timer_count[6] -= RSQ_UPDATE;
		}
		if(timer_count[7] >= RDS_UPDATE) { //TODO Rework to interrupt base system
			rds_update(&radio);
			timer_count[7] -= RDS_UPDATE;
		}
		if(timer_count[2] >= RADIO_TEXT_SCROLL) {
			radio.status.scroll_text++;
			if(radio.status.scroll_text == 78) {
				radio.status.scroll_text = 0;
			}
			timer_count[2] -= RADIO_TEXT_SCROLL;
		}
	}

}

#pragma vector=WDT_VECTOR
__interrupt void watchdog_timer(void){}

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
