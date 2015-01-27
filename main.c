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



#include <msp430.h>
#include <msp430g2553.h>
#include <intrinsics.h>
#include <stdio.h>
#include <stdlib.h>
#include "MSP430G2553_USCI_I2C.h"
#include "SI4735.h"
#include "MSP430G2553_Clock_Timer.h"
#include "MSP430G2553_GPIO.h"
//#include "AudioSwitch.h"
#include "Verstaerker.h"
#include "LCDDOGMA.h"
#include "Encoder.h"
#include "Timer.h"
#include "Menu.h"

volatile unsigned char sekunde = 0;
volatile unsigned char posrt = 0;
volatile unsigned char sec = 240;

int main (void)
{
	Clock_INIT2();
	WDTCTL = WDT_ADLY_250;                  // WDT 250ms, ACLK, interval timer
	IE1 |= WDTIE;							//WDT Interupt Enable

	//Warte bis alles mit Spannung versorgt ist und Empfangsbereit ist
	_delay_ms(250);

	i2c_lcd_brightnes(511);
	i2c_lcd_init();							//LCD Init

	i2c_lcd_generatebargraph();				//Gernarte GCCR Symbole

	//i2c_lcd_write_char('x');

	i2c_lcd_create_view(" ",0, 0, 1);		//Clear Display

	//i2c_lcd_write_char('x');

	i2c_lcd_create_view(Start_up_1, Shift_left_1, 0, 0);
	i2c_lcd_create_view(Start_up_2, Shift_left_2, 1, 0);
	i2c_lcd_create_view(Start_up_3, Shift_left_3, 2, 1);
	i2c_lcd_brightnes(511);
	//Setze den Input auf den SI4735
	//AudioSwitch(RADIO_LEFT,RADIO_RIGHT);

	//Schalte den Verstärker mit Default Werten ein
	Amplifier_init(POP,0);


	// Init I2C Kommunikation übergebe Adresse des Slaves 0x11h und den Teiler für die Frequenz
	USCI_I2C_INIT(0x11,40);

	// Initialisiert den Radiochip
	SI4735_INIT();

	//SI4735_Set_Volume(20);

	_delay_ms(30);

	Encoder_1_init();
	Encoder_2_init();
	Encoder_Timer_init();

	while(1)
	{
	  menu();
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
#pragma vector=TIMER0_A1_VECTOR
__interrupt void tim0(void){}
#pragma vector=TIMER1_A1_VECTOR
__interrupt void tim1(void){}
#pragma vector=USCIAB0RX_VECTOR
__interrupt void us_rx(void){}
#pragma vector=USCIAB0TX_VECTOR
__interrupt void us_tx(void){}
#pragma vector=ADC10_VECTOR
__interrupt void adc(void){}
#pragma vector=COMPARATORA_VECTOR
__interrupt void comp(void){}
#pragma vector=TIMER1_A0_VECTOR
__interrupt void Timer_A(void){}
