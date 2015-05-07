/*
 * tpa2016d2.h
 *
 *  Created on: 11.12.2013
 *      Author: Alexander
 */

#ifndef TPA2016D2_H_
#define TPA2016D2_H_

#include <msp430.h>
#include <stdint.h>
#include <driver/i2c.h>
#include <settings/radio_pin_mapping.h>
#include "Timer.h"

#define I2C_TPA2016D2_ADR		0x58

#define TPA2016D2_ATTACK_US_TO_BIN(x) (x / 1280)
#define TPA2016D2_RELEASE_US_TO_BIN(x) (x / 164000)
#define TPA2016D2_HOLD_US_TO_BIN(x) (x / 13700)
#define TPA2016D2_LIMTER_LEVEL_TO_BIN(x) ((2 * x + 13))

enum {
	TPA2016D2_COMPRESSION_1_1 = 0,
	TPA2016D2_COMPRESSION_2_1,
	TPA2016D2_COMPRESSION_4_1,
	TPA2016D2_COMPRESSION_8_1,
};

enum {
	TPA2016D2_NOISEGATE_1mV = 0,
	TPA2016D2_NOISEGATE_4mV,
	TPA2016D2_NOISEGATE_10mV,
	TPA2016D2_NOISEGATE_20mV,
};

enum TPA2016D2_REGISTER {
	TPA2016D2_CONTROLL = 1,
	TPA2016D2_ATTACK,
	TPA2016D2_RELEASE,
	TPA2016D2_HOLD,
	TPA2016D2_FGAIN,
	TPA2016D2_AGC_CONTROLL};

enum TPA2016D2_EQUALIZER {
	TPA2016D2_POP = 0,
	TPA2016D2_CLASSIC,
	TPA2016D2_JAZZ,
	TPA2016D2_RAP_HIP_HOP,
	TPA2016D2_ROCK,
	TPA2016D2_NEWS_VOICE
};

enum TPA2016D2_POWER_MODE {
	TPA2016D2_POWERUP = 0,
	TPA2016D2_SHUTDOWN
};

enum TPA2016D2_AUDIO_MODE {
	TPA2016D2_MUTE = 0,
	TPA2016D2_OUTPUT
};

typedef struct tpa2016d2_ctrl{
	uint8_t ng_en :1;
	uint8_t :1;
	uint8_t thermal :1;
	uint8_t fault_l :1;
	uint8_t fault_r :1;
	uint8_t sws :1;
	uint8_t spk_en_l :1;
	uint8_t spk_en_r :1;
} TPA2016D2_CTRL;

typedef struct tpa2016d2_agc_ctrl{
	uint8_t out_lim_lev :5;
	uint8_t noise_gate_thres :2;
	uint8_t out_lim_dis :1;
	uint8_t comp_ratio:2;
	uint8_t :2;
	uint8_t max_gain :4;
} TPA2016D2_AGC_CTRL;

typedef struct tpa2016d2_equalizer {
	uint8_t atk_time;
	uint8_t rel_time;
	uint8_t hold_time;
	uint8_t fixed_gain;
	TPA2016D2_AGC_CTRL agc_ctrl;
} TPA2016D2_EQUAL;

typedef struct tpa2016d2_config {
	TPA2016D2_CTRL ctrl;
	TPA2016D2_EQUAL equalizer;
} TPA2016D2_CONFIG;

uint8_t tpa2016d2_init(enum TPA2016D2_EQUALIZER equalizer, int8_t gain);

uint8_t tpa2016d2_powermode(enum TPA2016D2_POWER_MODE mode);

uint8_t tpa2016d2_muting(enum TPA2016D2_AUDIO_MODE state);

uint8_t tpa2016d2_equalizer_mode(enum TPA2016D2_EQUALIZER equalizer, int8_t gain);

#endif /* TPA2016D2_H_ */
