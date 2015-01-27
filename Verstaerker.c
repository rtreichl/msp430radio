/*
 * Verstaerker.c
 *
 *  Created on: 11.12.2013
 *      Author: Alexander
 */


//#define F_CPU 11059200UL

#include <msp430g2553.h>
#include "MSP430G2553_USCI_I2C.h"
#include <stdio.h>
#include "MSP430G2553_Clock_Timer.h"
#include "Verstaerker.h"
#include "Timer.h"


#define I2C_AUDIO_AMP			0x58
#define AUDIO_SHUT_DOWN_PORT	P3OUT
#define AUDIO_SHUT_DOWN_PIN		BIT2
#define AUDIO_SHUT_DOWN_DDR		P3DIR



void Amplifier_init(unsigned char Start_Mode,signed char Start_Gain)
{

	AUDIO_SHUT_DOWN_DDR |= AUDIO_SHUT_DOWN_PIN;			//Verstärker über SDZ einschalten
	AUDIO_SHUT_DOWN_PORT|= AUDIO_SHUT_DOWN_PIN;

	_delay_us(500);										//Warte bis der Vertärker empfangsbereit ist
						//Adresse des Verstäkers
	//Schalten den Verstäker für beide Lautspecher ein
	USCI_I2C_WRITE2(I2C_AUDIO_AMP, STOP, 2, 0x01, 0xC1);
	__delay_cycles(800);
	Amplifier_set_equalizer_mode(Start_Mode, Start_Gain); //Weitere einstellungen am Verstärker inklusive Verstärkung
}

void Amplifier_Shutdown(unsigned char Shutdown) 	//only 1 or 0 => 1 for Shutdown and 0 for Release
{
	USCI_I2C_WRITE2(I2C_AUDIO_AMP, STOP, 2, 0x01,0xC3 | Shutdown<<5);
}

void Amplifier_Gain(signed char Gain)
{
	if(Gain > 30)										//Überprüfe den werte bereich von Gain
		Gain = 30;
	if(Gain < -28)
		Gain = -28;
	USCI_I2C_WRITE2(I2C_AUDIO_AMP, STOP, 2, 0x05,(Gain & 0x3F));
}

void Amplifier_set_equalizer_mode(unsigned char Mode,signed char Gain)
{
	char Equalizer[7][7] = {
			0x02,0x03,0x08,0x0A,0x00,0x3C,0xC2,		// Pop
			0x02,0x02,0x07,0x0A,0x00,0x3D,0xC1,		// Classic
			0x02,0x06,0x14,0x00,0x00,0x3D,0xC1,		// Jazz
			0x02,0x01,0x0A,0x00,0x00,0x3C,0xC2,		// Rap / Hip Hop
			0x02,0x03,0x19,0x00,0x00,0x3D,0xC1,		// Rock
			0x02,0x02,0x0A,0x00,0x00,0x3E,0xC2,		// News / Voice
			0x02,0xCF,0x01,0x00,0x00,0x8E,0xC0		// Test

	};
	if(Gain > 30)									//Überprüfe den werte bereich von Gain
		Gain = 30;
	if(Gain < -28)
		Gain = -28;
	Equalizer[Mode][4] = 0x3F & Gain;					// Set Gain
	//USCI_I2C_INIT (0x58,40);
	USCI_I2C_WRITE2(I2C_AUDIO_AMP, STOP, 7, 0x02, 0x03, 0x08, 0x0A, Gain, 0x3C, 0xC2);
}


