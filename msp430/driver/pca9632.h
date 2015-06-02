/*
 * pca9632.h
 *
 *  Created on: 29.04.2015
 *      Author: Richard
 */

#ifndef DRIVER_PCA9632_H_
#define DRIVER_PCA9632_H_

#include <stdint.h>

#define PCA9632_I2C_ADR		0x62

enum PCA9632_AI_MODE {
	PCA9632_NO_AUTO_INCREMENT = 0,
	PCA9632_ALL_AUTO_INCREMENT = 4,
	PCA9632_CRTL_AUTO_INCREMENT = 5,
	PCA9632_INV_CTRL_AUTO_INCREMENT = 7
};

enum PCA9632_REGISTERS {
	PCA9632_MODE1 = 0,
	PCA9632_MODE2,
	PCA9632_PWM0,
	PCA9632_PWM1,
	PCA9632_PWM2,
	PCA9632_PWM3,
	PCA9632_GRPPWM,
	PCA9632_GRPFREQ,
	PCA9632_LEDOUT,
	PCA9632_SUBADR1,
	PCA9632_SUBADR2,
	PCA9632_SUBADR3,
	PCA9632_ALLCALLADR
};

enum PCA9632_LEDOUT_STATE {
	PCA9632_LED_OFF = 0,
	PCA9632_LED_ON,
	PCA9632_LED_ONEPWM,
	PCA9632_LED_GRPPWM
};

/*typedef struct pca9632_mode1 {
	uint8_t AI:3;
	uint8_t SLEEP:1;
	uint8_t SUB1:1;
	uint8_t SUB2:1;
	uint8_t SUB3:1;
	uint8_t ALLCALL:1;
} PCA9632_MODE1;

typedef struct pca9632_mode2 {
	uint8_t :2;
	uint8_t DMBLNK0:1;
	uint8_t INVRT:1;
	uint8_t OCH:1;
	uint8_t OUTDRV:1;
	uint8_t OUTNE:2;
} PCA9632_MODE2;

typedef struct pca9632_ledout {
	uint8_t LDR3:2;
	uint8_t LDR2:2;
	uint8_t LDR1:2;
	uint8_t LDR0:2;
} PCA9632_LEDOUT;

typedef struct pca9632_subadr {
	uint8_t ADR:7;
	uint8_t :1;
} PCA9632_SUBADR;*/

#endif /* DRIVER_PCA9632_H_ */
