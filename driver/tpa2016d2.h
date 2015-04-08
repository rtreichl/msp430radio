/*
 * Verstaerker.h
 *
 *  Created on: 11.12.2013
 *      Author: Alexander
 */

#ifndef VERSTAERKER_H_
#define VERSTAERKER_H_

#include <msp430.h>
#include <stdint.h>
#include <driver/i2c.h>
#include <settings/radio_pin_mapping.h>
#include "Timer.h"

#define I2C_TPA2016D2_ADR		0x58

enum EQUALIZER {POP = 0, CLASSIC, JAZZ, RAP_HIP_HOP, ROCK, NEWS_VOICE};

enum AMPLIFIER {RELEASE = 0, SHUTDOWN};

void tpa2016d2_init(uint8_t Start_Mode, int8_t Start_Gain);

void tpa2016d2_shutdown(uint8_t Shutdown);

void tpa2016d2_gain(int8_t Gain);

void tpa2016d2_equalizer_mode(uint8_t Mode, int8_t Gain);

#endif /* VERSTAERKER_H_ */
