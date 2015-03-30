/*
 * pca9530.c
 *
 *  Created on: 31.01.2015
 *      Author: Richard
 */

#include <driver/pca9530.h>
#include <settings/radio_pin_mapping.h>

uint8_t	pca9530_set_freq(enum PCA9530_FREQ freq_sel, uint8_t  freq)
{
	int8_t prescale = 0;
	PCA9530_CTL pca9530_ctl;

	pca9530_ctl.AI = 0;

	switch(freq_sel) {
	case FREQ_0:
		pca9530_ctl.ADDRESS = PSC0;
		break;
	case FREQ_1:
		pca9530_ctl.ADDRESS = PSC1;
		break;
	default:
		return 0xFF;
	}

	prescale = (15200 / (int16_t)freq) / 100 - 1;

	i2c_write_var(PCA9530_I2C_ADR, STOP, 2, pca9530_ctl.ADDRESS, prescale);
	//i2c_write_var(PCA9530_I2C_ADR, STOP, 2, pca9530_ctl.ADDRESS, prescale);

	return 0;
}

uint8_t pca9530_set_pwm(enum PCA9530_PWM pwm_sel, uint8_t pwm)
{
	PCA9530_CTL pca9530_ctl;

	pca9530_ctl.AI = 0;

	switch(pwm_sel) {
	case PWM_0:
		pca9530_ctl.ADDRESS = PWM0;
		break;
	case PWM_1:
		pca9530_ctl.ADDRESS = PWM1;
		break;
	default:
		return 0xFF;
	}

	i2c_write_var(PCA9530_I2C_ADR, STOP, 2, pca9530_ctl.ADDRESS, pwm);

	return 0;
}

uint8_t pca9530_config_leds( enum PCA9530_LED_STATE led0_state, enum PCA9530_LED_STATE led1_state)
{
	PCA9530_CTL pca9530_ctl;
	PCA9530_SEL pca9530_sel;

	pca9530_ctl.AI = 0;
	pca9530_ctl.ADDRESS = LS0;

	pca9530_sel.LED0 =  led0_state;
	pca9530_sel.LED1 =  led1_state;

	i2c_write_var(PCA9530_I2C_ADR, STOP, 2, pca9530_ctl.ADDRESS, pca9530_sel.LED0 | (pca9530_sel.LED1 << 2));

	return 0;
}

uint8_t pca9530_read_input()
{
	uint8_t input = 0;
	PCA9530_CTL pca9530_ctl;

	pca9530_ctl.AI = 0;
	pca9530_ctl.ADDRESS = INPUT;

	i2c_write_var(PCA9530_I2C_ADR, REPT, 1, pca9530_ctl.ADDRESS);
	i2c_read(PCA9530_I2C_ADR, STOP, 1, &input);

	return input;
}

uint8_t pca9530_init( PCA9530 *config)
{
	PWM_RST_DIR |= PWM_RST_PIN;
	PWM_RST_OUT |= PWM_RST_PIN;

	pca9530_set_pwm(PWM_0, config->pwm0);
	pca9530_set_pwm(PWM_1, config->pwm1);
	pca9530_set_freq(FREQ_0, config->freq0);
	pca9530_set_freq(FREQ_1, config->freq1);
	pca9530_config_leds(config->led0, config->led1);

	return 0;
}
