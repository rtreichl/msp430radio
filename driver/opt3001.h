/*
 * opt3001.h
 *
 *  Created on: 26.03.2015
 *      Author: Richard
 */

#ifndef OPT3001_H_
#define OPT3001_H_

#include <stdint.h>
#include <driver/i2c.h>

#define OPT3001_GND_PIN		0x00
#define OPT3001_VDD_PIN 	0x01
#define OPT3001_SDA_PIN		0x02
#define OPT3001_SCL_PIN		0x03
#define OPT3001_BASE_ADR	0x44
#define OPT3001_ADR_PIN		OPT3001_GND_PIN
#define OPT3001_I2C_ADR 	(OPT3001_BASE_ADR | OPT3001_ADR_PIN)

#define OPT3001_RESULT_REG	0x00
#define OPT3001_CONFIG_REG 	0x01
#define OPT3001_L_LIMIT_REG	0x02
#define OPT3001_H_LIMIT_REG 0x03
#define OPT3001_MANU_ID_REG	0x7E
#define OPT3001_DEV_ID_REG	0x7F

typedef struct opt3001_stc {
	uint16_t config;
	uint16_t low_limit;
	uint16_t high_limit;
} OPT3001_STC;

#define float_to_int(value) ((value & 0x0FFFF) << ((value & 0xF000) >> 12))

#endif /* OPT3001_H_ */
