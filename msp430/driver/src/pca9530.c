/*
 * pca9530.c
 *
 *  Created on: 31.01.2015
 *      Author: Richard
 */

#include <driver/pca9530.h>
#include <settings/radio_pin_mapping.h>

uint8_t pca9530_set_register(enum PCA9530_CTRL_CMD cmd, void *data)
{
	PCA9530_CTL pca9530_ctl;

	pca9530_ctl.ai = 0;

	pca9530_ctl.addr = cmd;

	i2c_write_var(PCA9530_I2C_ADR, STOP, 2, pca9530_ctl.addr, *((uint8_t *)data));

	return 0;
}

uint8_t pca9530_get_register(enum PCA9530_CTRL_CMD cmd, void *data)
{
	PCA9530_CTL pca9530_ctl;

	pca9530_ctl.ai = 0;
	pca9530_ctl.addr= cmd;

	i2c_write_arr(PCA9530_I2C_ADR, REPT, sizeof(pca9530_ctl), &pca9530_ctl);
	i2c_read(PCA9530_I2C_ADR, STOP, I2C_LITTLE_ENDIAN, 1, data);

	return 0;
}

uint8_t pca9530_init(const PCA9530 *config)
{
	PWM_RST_DIR |= PWM_RST_PIN;
	PWM_RST_OUT |= PWM_RST_PIN;

	i2c_write_arr(PCA9530_I2C_ADR, STOP, sizeof(PCA9530), config);
	return 0;
}
