/*
 * pca9534.h
 *
 *  Created on: 31.01.2015
 *      Author: Richard
 */

#ifndef PCA9534_H_
#define PCA9534_H_

#include <stdint.h>
#include <driver/i2c.h>

#define PCA9534_I2C_STD_ADR	0x20
#define PCA9534_I2C_A0_BIT 	0
#define PCA9534_I2C_A1_BIT 	0
#define PCA9534_I2C_A2_BIT 	0
#define PCA9534_I2C_ADR		(PCA9534_I2C_STD_ADR | PCA9534_I2C_A0_BIT | (PCA9534_I2C_A1_BIT << 1) | (PCA9534_I2C_A2_BIT << 2))

enum PCA9534_CTRL_CMD {
	IN = 0,
	OUTP,
	POL_INV,
	CONFIG
};

enum PCA9534_STAT {
	LOW = 0,
	HIGH
};

enum PCA9534_CONF {
	CONF_OUTPUT = 0,
	CONF_INPUT
};

uint8_t	pca9534_output(uint8_t  byte);
uint8_t pca9534_config(uint8_t config);
uint8_t pca9534_input();

#endif /* PCA9534_H_ */
