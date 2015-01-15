/*
 * Verstaerker.h
 *
 *  Created on: 11.12.2013
 *      Author: Alexander
 */

#ifndef VERSTAERKER_H_
#define VERSTAERKER_H_

enum EQUALIZER {POP, CLASSIC, JAZZ, RAP_HIP_HOP, ROCK, NEWS_VOICE};

void Amplifier_init(unsigned char Start_Mode,signed  char Start_Gain);

void Amplifier_Shutdown(unsigned char Shutdown);

void Amplifier_Gain(signed char Gain);

void Amplifier_set_equalizer_mode(unsigned char Mode,signed char Gain);

#endif /* VERSTAERKER_H_ */
