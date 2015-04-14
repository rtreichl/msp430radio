/*
 * Verstaerker.c
 *
 *  Created on: 11.12.2013
 *      Author: Alexander
 */
#include <driver/tpa2016d2.h>

void tpa2016d2_init(uint8_t Start_Mode, int8_t Start_Gain)
{
	// Enable Amplifier by pulling SDZ high
	AMP_SHUTDOWN_DIR |= AMP_SHUTDOWN_PIN;
	AMP_SHUTDOWN_OUT |= AMP_SHUTDOWN_PIN;

	// Wait some time before configure Amplifier
	_delay_ms(5);
	tpa2016d2_shutdown(1);

	// Wait some time before continue with configure
	__delay_cycles(800);

	// configure exended configuration
	tpa2016d2_equalizer_mode(Start_Mode, Start_Gain);

	// Enable Amplifier with R/L speaker enabled
	i2c_write_var(I2C_TPA2016D2_ADR, STOP, 2, 0x01, 0xC3);
}

void tpa2016d2_shutdown(uint8_t Shutdown) 	//only 1 or 0 => 1 for Shutdown and 0 for Release
{
	// Send command to amplifier
	i2c_write_var(I2C_TPA2016D2_ADR, STOP, 2, 0x01, 0xC3 | Shutdown<<5);
}

void tpa2016d2_mute(uint8_t mute) 	//only 1 or 0 => 1 for mute and 0 for unmute
{
	// Send command to amplifier
	if(mute == 1) {
		i2c_write_var(I2C_TPA2016D2_ADR, STOP, 2, 0x01, 0x03);
	}
	else {
		i2c_write_var(I2C_TPA2016D2_ADR, STOP, 2, 0x01, 0xC3);
	}
}

void tpa2016d2_gain(int8_t Gain)
{
	// Check gain value for valid range
	if(Gain > 30)
		Gain = 30;
	if(Gain < -28)
		Gain = -28;

	// Send new gain to amplifier
	i2c_write_var(I2C_TPA2016D2_ADR, STOP, 2, 0x05,(Gain & 0x3F));
}

void tpa2016d2_equalizer_mode(uint8_t Mode, int8_t Gain)
{
	// Check gain value for valid range
	if(Gain > 30)
		Gain = 30;
	if(Gain < -28)
		Gain = -28;
	switch(Mode) {
	case 0:
		// POP
		i2c_write_var(I2C_TPA2016D2_ADR, STOP, 7, 0x02, 0x03, 0x08, 0x0A, (Gain & 0x3F), 0x5C, 0xC0);
		break;
	case 1:
		// CLASSIC
		i2c_write_var(I2C_TPA2016D2_ADR, STOP, 7, 0x02, 0x02, 0x07, 0x0A, (Gain & 0x3F), 0x5D, 0xC0);
		break;
	case 2:
		// JAZZ
		i2c_write_var(I2C_TPA2016D2_ADR, STOP, 7, 0x02, 0x06, 0x14, 0x00, (Gain & 0x3F), 0x5D, 0xC0);
		break;
	case 3:
		// RAP HIP HOP
		i2c_write_var(I2C_TPA2016D2_ADR, STOP, 7, 0x02, 0x01, 0x0A, 0x00, (Gain & 0x3F), 0x5C, 0xC0);
		break;
	case 4:
		// ROCK
		i2c_write_var(I2C_TPA2016D2_ADR, STOP, 7, 0x02, 0x03, 0x19, 0x00, (Gain & 0x3F), 0x5D, 0xC0);
		break;
	case 5:
		// NEWS VOICE
		i2c_write_var(I2C_TPA2016D2_ADR, STOP, 7, 0x02, 0x02, 0x0A, 0x00, (Gain & 0x3F), 0x5E, 0xC0);
		break;
	}
}


