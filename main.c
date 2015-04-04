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
#include <system/rsq.h>
#include <system/rds.h>
#include <driver/timer.h>

#define ENCODER_TAST_REFRESH	10
#define	TIME_MINUTE	60000
#define RSQ_UPDATE	1000
#define RDS_UPDATE	200

extern volatile unsigned char encoder_1_button, encoder_2_button;

#define DISPLAY_REFRESH 500

int main (void)
{
	WDTCTL = WDTPW + WDTHOLD;
	radio_init();
	int8_t en_counter1 = 0;
	int8_t en_counter2 = 0;
	time_set(18,29,27,2,16,0);

	while(1)
	{
		//menu();
		en_counter2 +=  Encoder_2_get_count();
		en_counter1 +=  Encoder_1_get_count();
		if(encoder_1_button != BUTTON_FREE || en_counter1 != 0 || encoder_2_button != BUTTON_FREE || en_counter2 != 0 || timer_count[4] >= DISPLAY_REFRESH) {
			menu_handler(&encoder_1_button, &en_counter1, &encoder_2_button, &en_counter2);
			timer_count[4] = 0;
		}
		if(timer_count[1] >= ENCODER_TAST_REFRESH) {
			timer_count[1] -= ENCODER_TAST_REFRESH;
			encoder_interrupt2();
		}
		if(timer_count[3] >= TIME_MINUTE) {
			timer_count[3] -= TIME_MINUTE;
			time_date_update();
		}
		if(timer_count[6] >= RSQ_UPDATE) {
			rsq_update(&radio);
			timer_count[6] -= RSQ_UPDATE;
		}
		if(timer_count[7] >= RDS_UPDATE) { //TODO Rework to interrupt base system
			rds_update(&radio);
			timer_count[7] -= RDS_UPDATE;
		}
		//encoder_2_button = 'f';
		//en_counter2 = 0;
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
