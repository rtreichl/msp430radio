/*
 * Verstaerker.c
 *
 *  Created on: 11.12.2013
 *      Author: Alexander
 */
#include <driver/tpa2016d2.h>

void Amplifier_init(uint8_t Start_Mode, int8_t Start_Gain)
{
	// Enable Amplifier by pulling SDZ high
	AMP_SHUTDOWN_DIR |= AMP_SHUTDOWN_PIN;
	AMP_SHUTDOWN_OUT |= AMP_SHUTDOWN_PIN;

	// Wait some time before configure Amplifier
	_delay_ms(5);


	// Enable Amplifier with R/L speaker enabled
	i2c_write_var(I2C_AUDIO_AMP, STOP, 2, 0x01, 0xC1);

	// Wait some time before continue with configure
	__delay_cycles(800);

	// configure exended configuration
	Amplifier_set_equalizer_mode(Start_Mode, Start_Gain);
}

void Amplifier_Shutdown(uint8_t Shutdown) 	//only 1 or 0 => 1 for Shutdown and 0 for Release
{
	// Send command to amplifier
	i2c_write_var(I2C_AUDIO_AMP, STOP, 2, 0x01, 0xC3 | Shutdown<<5);
}

void Amplifier_Gain(int8_t Gain)
{
	// Check gain value for valid range
	if(Gain > 30)
		Gain = 30;
	if(Gain < -28)
		Gain = -28;

	// Send new gain to amplifier
	i2c_write_var(I2C_AUDIO_AMP, STOP, 2, 0x05,(Gain & 0x3F));
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
		i2c_write_var(I2C_AUDIO_AMP, STOP, 7, 0x02, 0x03, 0x08, 0x0A, (Gain & 0x3F), 0x3C, 0xC2);
		break;
	case 1:
		// CLASSIC
		i2c_write_var(I2C_AUDIO_AMP, STOP, 7, 0x02, 0x02, 0x07, 0x0A, (Gain & 0x3F), 0x3D, 0xC1);
		break;
	case 2:
		// JAZZ
		i2c_write_var(I2C_AUDIO_AMP, STOP, 7, 0x02, 0x06, 0x14, 0x00, (Gain & 0x3F), 0x3D, 0xC1);
		break;
	case 3:
		// RAP HIP HOP
		i2c_write_var(I2C_AUDIO_AMP, STOP, 7, 0x02, 0x01, 0x0A, 0x00, (Gain & 0x3F), 0x3C, 0xC2);
		break;
	case 4:
		// ROCK
		i2c_write_var(I2C_AUDIO_AMP, STOP, 7, 0x02, 0x03, 0x19, 0x00, (Gain & 0x3F), 0x3D, 0xC1);
		break;
	case 5:
		// NEWS VOICE
		i2c_write_var(I2C_AUDIO_AMP, STOP, 7, 0x02, 0x02, 0x0A, 0x00, (Gain & 0x3F), 0x3E, 0xC2);
		break;
	}
}


