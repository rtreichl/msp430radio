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
#include "MSP430G2553_USCI_I2C.h"
#include "Timer.h"

#define I2C_AUDIO_AMP			0x58
#define AUDIO_SHUT_DOWN_PORT	P3OUT
#define AUDIO_SHUT_DOWN_PIN		BIT2
#define AUDIO_SHUT_DOWN_DDR		P3DIR

enum EQUALIZER {POP = 0, CLASSIC, JAZZ, RAP_HIP_HOP, ROCK, NEWS_VOICE};

enum AMPLIFIER {RELEASE = 0, SHUTDOWN};

void Amplifier_init(uint8_t Start_Mode, int8_t Start_Gain);

void Amplifier_Shutdown(uint8_t Shutdown);

void Amplifier_Gain(int8_t Gain);

void Amplifier_set_equalizer_mode(uint8_t Mode, int8_t Gain);

#endif /* VERSTAERKER_H_ */
