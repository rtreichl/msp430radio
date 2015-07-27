/*
 * pca9534.c
 *
 *  Created on: 21.01.2014
 *      Author: Richi
 */

#include <driver/pca9534.h>

uint8_t	pca9534_output(uint8_t  byte)
{
	i2c_write_var(PCA9534_I2C_ADR, STOP, 2, OUTP, byte);
	return 0;
}
uint8_t pca9534_config(uint8_t config)
{
	i2c_write_var(PCA9534_I2C_ADR, STOP, 2, CONFIG, config);
	return 0;
}
uint8_t pca9534_input()
{
	uint8_t input;
	i2c_write_var(PCA9534_I2C_ADR, REPT, 1, IN);
	i2c_read(PCA9534_I2C_ADR, STOP, I2C_LITTLE_ENDIAN, 1, &input);
	return input;
}
