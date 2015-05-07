/*
 * tpa2016d2.c
 *
 *  Created on: 11.12.2013
 *      Author: Alexander
 */

#include <driver/tpa2016d2.h>

const TPA2016D2_EQUAL tpa2016d2_pop = {
		TPA2016D2_ATTACK_US_TO_BIN(1280),
		TPA2016D2_RELEASE_US_TO_BIN(1640000),
		TPA2016D2_HOLD_US_TO_BIN(137000),
		RADIO_AMPLIFIER_GAIN,
		{
				TPA2016D2_LIMTER_LEVEL_TO_BIN(7.5),
				2,
				TPA2016D2_NOISEGATE_10mV,
				TPA2016D2_COMPRESSION_1_1,
				12
		}
};

const TPA2016D2_EQUAL tpa2016d2_classic = {
		TPA2016D2_ATTACK_US_TO_BIN(2560),
		TPA2016D2_RELEASE_US_TO_BIN(1150000),
		TPA2016D2_HOLD_US_TO_BIN(137000),
		RADIO_AMPLIFIER_GAIN,
		{
				TPA2016D2_LIMTER_LEVEL_TO_BIN(8),
				2,
				TPA2016D2_NOISEGATE_10mV,
				TPA2016D2_COMPRESSION_1_1,
				12
		}
};

const TPA2016D2_EQUAL tpa2016d2_jazz = {
		TPA2016D2_ATTACK_US_TO_BIN(10200),
		TPA2016D2_RELEASE_US_TO_BIN(3288000),
		TPA2016D2_HOLD_US_TO_BIN(0),
		RADIO_AMPLIFIER_GAIN,
		{
				TPA2016D2_LIMTER_LEVEL_TO_BIN(8),
				2,
				TPA2016D2_NOISEGATE_10mV,
				TPA2016D2_COMPRESSION_1_1,
				12
		}
};

const TPA2016D2_EQUAL tpa2016d2_rap_hip_hop = {
		TPA2016D2_ATTACK_US_TO_BIN(2560),
		TPA2016D2_RELEASE_US_TO_BIN(1640000),
		TPA2016D2_HOLD_US_TO_BIN(0),
		RADIO_AMPLIFIER_GAIN,
		{
				TPA2016D2_LIMTER_LEVEL_TO_BIN(7.5),
				2,
				TPA2016D2_NOISEGATE_10mV,
				TPA2016D2_COMPRESSION_1_1,
				12
		}
};

const TPA2016D2_EQUAL tpa2016d2_rock = {
		TPA2016D2_ATTACK_US_TO_BIN(3840),
		TPA2016D2_RELEASE_US_TO_BIN(4110000),
		TPA2016D2_HOLD_US_TO_BIN(137),
		RADIO_AMPLIFIER_GAIN,
		{
				TPA2016D2_LIMTER_LEVEL_TO_BIN(8),
				2,
				TPA2016D2_NOISEGATE_10mV,
				TPA2016D2_COMPRESSION_1_1,
				12
		}
};

const TPA2016D2_EQUAL tpa2016d2_voice_news = {
		TPA2016D2_ATTACK_US_TO_BIN(2560),
		TPA2016D2_RELEASE_US_TO_BIN(1640000),
		TPA2016D2_HOLD_US_TO_BIN(0),
		RADIO_AMPLIFIER_GAIN,
		{
				TPA2016D2_LIMTER_LEVEL_TO_BIN(8.5),
				2,
				TPA2016D2_NOISEGATE_10mV,
				TPA2016D2_COMPRESSION_1_1,
				12
		}
};

const TPA2016D2_EQUAL * const tpa2016d2_equalizer[6] = {
		&tpa2016d2_pop,
		&tpa2016d2_classic,
		&tpa2016d2_jazz,
		&tpa2016d2_rap_hip_hop,
		&tpa2016d2_rock,
		&tpa2016d2_voice_news,
};




uint8_t tpa2016d2_init(enum TPA2016D2_EQUALIZER equalizer, int8_t gain)
{
	// Enable Amplifier by pulling SDZ high
	AMP_SHUTDOWN_DIR |= AMP_SHUTDOWN_PIN;
	AMP_SHUTDOWN_OUT |= AMP_SHUTDOWN_PIN;

	// Wait some time before configure Amplifier
	_delay_ms(5);

	// configure exended configuration
	tpa2016d2_equalizer_mode(equalizer, gain);

	// Enable Amplifier with R/L speaker enabled
	i2c_write_var(I2C_TPA2016D2_ADR, STOP, 2, 0x01, 0xC3);

	return 0;
}

uint8_t tpa2016d2_powermode(enum TPA2016D2_POWER_MODE mode)
{
	// Send command to amplifier
	i2c_write_var(I2C_TPA2016D2_ADR, STOP, 2, 0x01, 0xC3 | mode<<5);

	return 0;
}

uint8_t tpa2016d2_muting(enum TPA2016D2_AUDIO_MODE state)
{
	struct  {
		uint8_t cmd;
		TPA2016D2_CTRL controll;
	} tpa2016d2;

	tpa2016d2.cmd = TPA2016D2_CONTROLL;

	tpa2016d2.controll.ng_en = 1;

	// Send command to amplifier
	if(state == TPA2016D2_MUTE) {
		tpa2016d2.controll.spk_en_l = 0;
		tpa2016d2.controll.spk_en_r = 0;
	}
	else {
		tpa2016d2.controll.spk_en_l = 1;
		tpa2016d2.controll.spk_en_r = 1;
	}
	i2c_write_arr(I2C_TPA2016D2_ADR, STOP, sizeof(tpa2016d2), &tpa2016d2);

	return 0;
}

uint8_t tpa2016d2_equalizer_mode(enum TPA2016D2_EQUALIZER equalizer, int8_t gain)
{
	struct {
		uint8_t cmd;
		TPA2016D2_EQUAL equalizer;
	} tpa2016d2;

	tpa2016d2.cmd = TPA2016D2_ATTACK;

	memcpy(&(tpa2016d2.equalizer), tpa2016d2_equalizer[equalizer], sizeof(TPA2016D2_EQUAL));

	i2c_write_arr(I2C_TPA2016D2_ADR, STOP, sizeof(tpa2016d2), &tpa2016d2);

	return 0;
}


