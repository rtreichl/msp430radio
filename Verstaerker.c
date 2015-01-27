/*
 * Verstaerker.c
 *
 *  Created on: 11.12.2013
 *      Author: Alexander
 */
#include "Verstaerker.h"

void Amplifier_init(uint8_t Start_Mode, int8_t Start_Gain)
{
	// Enable Amplifier by pulling SDZ high
	AUDIO_SHUT_DOWN_DDR |= AUDIO_SHUT_DOWN_PIN;
	AUDIO_SHUT_DOWN_PORT|= AUDIO_SHUT_DOWN_PIN;

	// Wait some time before configure Amplifier
	_delay_us(500);


	// Enable Amplifier with R/L speaker enabled
	USCI_I2C_WRITE2(I2C_AUDIO_AMP, STOP, 2, 0x01, 0xC1);

	// Wait some time before continue with configure
	__delay_cycles(800);

	// configure exended configuration
	Amplifier_set_equalizer_mode(Start_Mode, Start_Gain);
}

void Amplifier_Shutdown(uint8_t Shutdown) 	//only 1 or 0 => 1 for Shutdown and 0 for Release
{
	// Send command to amplifier
	USCI_I2C_WRITE2(I2C_AUDIO_AMP, STOP, 2, 0x01,0xC3 | Shutdown<<5);
}

void Amplifier_Gain(int8_t Gain)
{
	// Check gain value for valid range
	if(Gain > 30)
		Gain = 30;
	if(Gain < -28)
		Gain = -28;

	// Send new gain to amplifier
	USCI_I2C_WRITE2(I2C_AUDIO_AMP, STOP, 2, 0x05,(Gain & 0x3F));
}

void Amplifier_set_equalizer_mode(uint8_t Mode, int8_t Gain)
{
	// Check gain value for valid range
	if(Gain > 30)
		Gain = 30;
	if(Gain < -28)
		Gain = -28;
	switch(Mode) {
	case 0:
		// POP
		USCI_I2C_WRITE2(I2C_AUDIO_AMP, STOP, 7, 0x02, 0x03, 0x08, 0x0A, (Gain & 0x3F), 0x3C, 0xC2);
		break;
	case 1:
		// CLASSIC
		USCI_I2C_WRITE2(I2C_AUDIO_AMP, STOP, 7, 0x02, 0x02, 0x07, 0x0A, (Gain & 0x3F), 0x3D, 0xC1);
		break;
	case 2:
		// JAZZ
		USCI_I2C_WRITE2(I2C_AUDIO_AMP, STOP, 7, 0x02, 0x06, 0x14, 0x00, (Gain & 0x3F), 0x3D, 0xC1);
		break;
	case 3:
		// RAP HIP HOP
		USCI_I2C_WRITE2(I2C_AUDIO_AMP, STOP, 7, 0x02, 0x01, 0x0A, 0x00, (Gain & 0x3F), 0x3C, 0xC2);
		break;
	case 4:
		// ROCK
		USCI_I2C_WRITE2(I2C_AUDIO_AMP, STOP, 7, 0x02, 0x03, 0x19, 0x00, (Gain & 0x3F), 0x3D, 0xC1);
		break;
	case 5:
		// NEWS VOICE
		USCI_I2C_WRITE2(I2C_AUDIO_AMP, STOP, 7, 0x02, 0x02, 0x0A, 0x00, (Gain & 0x3F), 0x3E, 0xC2);
		break;
	}
}


