/*
 * pca9632.c
 *
 *  Created on: 29.04.2015
 *      Author: Richard
 */

#include <driver/driver.h>

uint8_t pca9632_init(PCA9632 *config)
{
	PCA_ON_OFF_SEL &= (~PCA_ON_OFF_PIN);
	PCA_ON_OFF_DIR |= PCA_ON_OFF_PIN;
	PCA_ON_OFF_OUT |= PCA_ON_OFF_PIN;

	i2c_write_arr(PCA9632_I2C_ADR, STOP, sizeof(PCA9632), config);
	//i2c_write_var(PCA9632_I2C_ADR, STOP, 10, 0x80, 0x81, 0x15, 0x7F, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x02);
	//i2c_write_arr(PCA9632_I2C_ADR, STOP, sizeof(mode1), &mode1);
	return 0;
}

uint8_t pca9632_set_register(enum PCA9632_REGISTERS cmd, void *data)
{
	i2c_write_var(PCA9632_I2C_ADR, STOP, 2, cmd, *((uint8_t *)data));

	return 0;
}

uint8_t pca9632_get_register(enum PCA9632_REGISTERS cmd, void *data)
{
	i2c_write_arr(PCA9632_I2C_ADR, REPT, sizeof(cmd), cmd);
	i2c_read(PCA9632_I2C_ADR, STOP, I2C_LITTLE_ENDIAN, 1, data);

	return 0;
}
