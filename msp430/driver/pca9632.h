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

#define INVERT 1
#define NON_INVERT 0

#define OUT_DRV 1
#define NON_OUTDRV 0

#define GROUP_BLINK 1
#define GROUP_DIM 0

#define CHG_AT_STOP 0
#define CHG_AT_ACK 1

#define OUT_NE 1

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

enum PCA9632_AI_MODE {
	PCA9632_NO_AUTO_INCREMENT = 0,
	PCA9632_ALL_AUTO_INCREMENT = 4,
	PCA9632_CRTL_AUTO_INCREMENT = 5,
	PCA9632_INV_CTRL_AUTO_INCREMENT = 7
};


typedef struct pca9632_mode1 {
	uint8_t ALLCALL:1;
	uint8_t SUB3:1;
	uint8_t SUB2:1;
	uint8_t SUB1:1;
	uint8_t SLEEP:1;
	uint8_t AI:3;
} PCA9632_MODE1_STC;

typedef struct pca9632_register {
	uint8_t D:4;
	uint8_t :1;
	uint8_t AI:3;
} PCA9632_REGISTER_STC;

typedef struct pca9632_mode2 {
	uint8_t OUTNE:2;
	uint8_t OUTDRV:1;
	uint8_t OCH:1;
	uint8_t INVRT:1;
	uint8_t DMBLNK0:1;
	uint8_t :2;
} PCA9632_MODE2_STC;

typedef struct pca9632_ledout {
	uint8_t LDR0:2;
	uint8_t LDR1:2;
	uint8_t LDR2:2;
	uint8_t LDR3:2;
} PCA9632_LEDOUT_STC;

typedef struct pca9632_subadr {
	uint8_t :1;
	uint8_t ADR:7;
} PCA9632_SUBADR_STC;

typedef struct pca9632 {
	PCA9632_REGISTER_STC reg;
	PCA9632_MODE1_STC mode_1;
	PCA9632_MODE2_STC mode_2;
	uint8_t pwm_0;
	uint8_t pwm_1;
	uint8_t pwm_2;
	uint8_t pwm_3;
	uint8_t grp_pwm;
	uint8_t grp_freq;
	PCA9632_LEDOUT_STC led_out;
	PCA9632_SUBADR_STC sub_adr_1;
	PCA9632_SUBADR_STC sub_adr_2;
	PCA9632_SUBADR_STC sub_adr_3;
	PCA9632_SUBADR_STC all_call_adr;
} PCA9632;

uint8_t pca9632_init();
uint8_t pca9632_set_register(enum PCA9632_REGISTERS cmd, void *data);
uint8_t pca9632_get_register(enum PCA9632_REGISTERS cmd, void *data);
#endif /* DRIVER_PCA9632_H_ */
