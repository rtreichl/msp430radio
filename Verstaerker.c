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


#define I2C_AUDIO_AMP			0xB0
#define AUDIO_SHUT_DOWN_PORT	P3OUT
#define AUDIO_SHUT_DOWN_PIN		BIT2
#define AUDIO_SHUT_DOWN_DDR		P3DIR



void Amplifier_init(unsigned char Start_Mode,signed char Start_Gain)
{
	char Kommando[3];

	AUDIO_SHUT_DOWN_DDR |= AUDIO_SHUT_DOWN_PIN;			//Verst�rker �ber SDZ einschalten
	AUDIO_SHUT_DOWN_PORT|= AUDIO_SHUT_DOWN_PIN;

	_delay_us(500);										//Warte bis der Vert�rker empfangsbereit ist

	USCI_I2C_INIT (0x58,40);							//Adresse des Verst�kers
	sprintf(Kommando, "%c%c", 0x01,0xC1);				//Schalten den Verst�ker f�r beide Lautspecher ein
	USCI_I2C_WRITE1(2, Kommando);
	__delay_cycles(800);
	Amplifier_set_equalizer_mode(Start_Mode, Start_Gain); //Weitere einstellungen am Verst�rker inklusive Verst�rkung
}

void Amplifier_Shutdown(unsigned char Shutdown) 	//only 1 or 0 => 1 for Shutdown and 0 for Release
{
	char Kommando[3];
	USCI_I2C_INIT (0x58,40);
	sprintf(Kommando, "%c%c", 0x01,0xC3 | Shutdown<<5);
	USCI_I2C_WRITE1(2, Kommando);
}

void Amplifier_Gain(signed char Gain)
{
	char Kommando[3];
	if(Gain > 30)										//�berpr�fe den werte bereich von Gain
		Gain = 30;
	if(Gain < -28)
		Gain = -28;
	USCI_I2C_INIT (0x58,40);
	sprintf(Kommando, "%c%c", 0x05,(Gain & 0x3F));
	USCI_I2C_WRITE1(2, Kommando);
}

void Amplifier_set_equalizer_mode(unsigned char Mode,signed char Gain)
{
	char Equalizer[6][7] = {
			0x02,0x03,0x08,0x0A,0x00,0x3C,0xC2,		// Pop
			0x02,0x02,0x07,0x0A,0x00,0x3D,0xC1,		// Classic
			0x02,0x06,0x14,0x00,0x00,0x3D,0xC1,		// Jazz
			0x02,0x01,0x0A,0x00,0x00,0x3C,0xC2,		// Rap / Hip Hop
			0x02,0x03,0x19,0x00,0x00,0x3D,0xC1,		// Rock
			0x02,0x02,0x0A,0x00,0x00,0x3E,0xC2		// News / Voice
	};
	if(Gain > 30)									//�berpr�fe den werte bereich von Gain
		Gain = 30;
	if(Gain < -28)
		Gain = -28;
	Equalizer[Mode][4] = 0x3F & Gain;					// Set Gain
	USCI_I2C_INIT (0x58,40);
	USCI_I2C_WRITE1(7, Equalizer[Mode]);
}


