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


enum PCA9530_CTRL_CMD {
	INPUT = 0,
	PSC0,
	PWM0,
	PSC1,
	PWM1,
	LS0
};

enum PCA9530_LED_STATE {
	LED_OFF = 0,
	LED_ON,
	PWM0_RATE,
	PWM1_RATE
};

enum PCA9530_FREQ {
	FREQ_0 = 0,
	FREQ_1
};

enum PCA9530_PWM {
	PWM_0 = 0,
	PWM_1
};

typedef struct  pca9530_ctl {
	uint8_t : 3;
	uint8_t	AI : 1;
	uint8_t	: 1;
	uint8_t	ADDRESS : 3;
} PCA9530_CTL;

typedef struct pca9530_in {
	uint8_t	: 6;
	uint8_t	LED1 : 1;
	uint8_t	LED0 : 1;
} PCA9530_IN;

typedef struct pca9530_sel {
	uint8_t	: 4;
	uint8_t	LED1 : 2;
	uint8_t	LED0 : 2;
} PCA9530_SEL;

typedef struct pca9530 {
	uint8_t freq0;
	uint8_t pwm0;
	uint8_t freq1;
	uint8_t pwm1;
	PCA9530_SEL led_sel;
} PCA9530;

uint8_t	pca9530_set_freq(enum PCA9530_FREQ freq_sel, uint8_t  freq);
uint8_t pca9530_set_pwm(enum PCA9530_PWM pwm_sel, uint8_t pwm);
uint8_t pca9530_config_led(enum PCA9530_LED_STATE led_state);
uint8_t pca9530_read_input();
uint8_t pca9530_init( PCA9530 *config);

#endif /* PCA9530_H_ */
