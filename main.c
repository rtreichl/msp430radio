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
#define Start_up_1 		"HS-Rosenheim"
#define	Shift_left_1	2
#define	Start_up_2		"FM/AM  Radio"
#define	Shift_left_2	2
#define Start_up_3		"Version 1.00"
#define	Shift_left_3	2

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

#define AMPLIFIER_GAIN 15

volatile unsigned char sekunde = 0;
volatile unsigned char posrt = 0;
volatile unsigned char sec = 240;

extern volatile unsigned char encoder_1_button, encoder_2_button, sekunde, sec , posrt;

int main (void)
{
	basic_clock_init();
	timer_init();
	i2c_init (400,10);
	WDTCTL = WDTPW + WDTHOLD;

	//WDTCTL = WDT_ARST_1000;                  // WDT 250ms, ACLK, interval timer
	//IE1 |= WDTIE;							//WDT Interupt Enable
	//_EINT();
	//while(1);
	//Warte bis alles mit Spannung versorgt ist und Empfangsbereit ist
	_delay_ms(100);

	const PCA9530 config = {
		76,
		0,
		1,
		127,
		PWM0_RATE,
		PWM1_RATE
	};

	pca9530_init(&config);

	lcd_init(4);							//LCD Init

	radio_brightness(90);

	//Clear Display

	//i2c_lcd_write_char('x');

	lcd_create_view(Start_up_1, Shift_left_1, 0, 0);
	lcd_create_view(Start_up_2, Shift_left_2, 1, 0);
	lcd_create_view(Start_up_3, Shift_left_3, 2, 1);
	//i2c_lcd_brightnes(511);
	//Setze den Input auf den SI4735
	//AudioSwitch(RADIO_LEFT,RADIO_RIGHT);

	// Initialisiert den Radiochip
	SI4735_INIT();

	//Schalte den Verstärker mit Default Werten ein
	Amplifier_init(POP,AMPLIFIER_GAIN);

	//SI4735_Set_Volume(20);

	_delay_ms(30);

	//SI4735_Fm_Tune_Status();

	Encoder_1_init();
	Encoder_2_init();
	Encoder_Timer_init();

	int8_t en_counter2;


	while(1)
	{
		//menu();
	  en_counter2 +=  Encoder_2_get_count();
	  menu_handler(&encoder_2_button, &en_counter2);
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
		time_date(0, 0, 0, 0, 0, 0, 1, 0, 0);
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
