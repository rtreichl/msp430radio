/*
 * opt3001.h
 *
 *  Created on: 26.03.2015
 *      Author: Richard
 */

#ifndef OPT3001_H_
#define OPT3001_H_

#include <stdint.h>

enum { RESULT  = 0x00, CONFIG = 0x01, LOW_LIMIT = 0x02, HIGH_LIMIT = 0x03, MAN_ID = 0x7E, DEV_ID = 0x7F};

typedef struct opt3001_stc {
	uint16_t config;
	uint16_t low_limit;
	uint16_t high_limit;
} OPT3001_STC;

#define float_to_int(value) ((value 0x0FFFF) << ((value & 0xF000) >> 12))

#endif /* OPT3001_H_ */
