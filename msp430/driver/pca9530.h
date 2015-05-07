/*
 * pca9530.h
 *
 *  Created on: 31.01.2015
 *      Author: Richard
 */

#ifndef PCA9530_H_
#define PCA9530_H_

#include <stdint.h>
#include <driver/i2c.h>

#define PCA9530_I2C_STD_ADR	0x60
#define PCA9530_I2C_A0_BIT 	0
#define PCA9530_I2C_ADR		(PCA9530_I2C_STD_ADR | PCA9530_I2C_A0_BIT)

#define PCA9530_FREQ_TO_PRESCALE(x) ((15200 / x) / 100 - 1)

enum PCA9530_CTRL_CMD {
	INPUT = 0,
	PSC0,
	PWM0,
	PSC1,
	PWM1,
	LS0,
};

enum PCA9530_LED_STATE {
	LED_OFF = 0,
	LED_ON,
	PWM0_RATE,
	PWM1_RATE
};

typedef struct pca9530_ls0 {
	uint8_t led0:2;
	uint8_t led1:2;
	uint8_t :4;
} PCA9530_LS0;

typedef struct  pca9530_ctl {
	uint8_t : 3;
	uint8_t	ai : 1;
	uint8_t	: 1;
	uint8_t	addr : 3;
} PCA9530_CTL;

typedef struct pca9530_in {
	uint8_t	: 6;
	uint8_t	led1 : 1;
	uint8_t	led0 : 1;
} PCA9530_IN;

typedef struct pca9530 {
	uint8_t addr;
	uint8_t freq0;
	uint8_t pwm0;
	uint8_t freq1;
	uint8_t pwm1;
	PCA9530_LS0 ls0;
} PCA9530;

uint8_t pca9530_set_register(enum PCA9530_CTRL_CMD cmd, void *data);
uint8_t pca9530_get_register(enum PCA9530_CTRL_CMD cmd, void *data);
uint8_t pca9530_init(const PCA9530 *config);

#endif /* PCA9530_H_ */
