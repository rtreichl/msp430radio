/*
 * opt3001.c
 *
 *  Created on: 26.03.2015
 *      Author: Richard
 */

#include <driver/opt3001.h>

uint8_t opt3001_init(const OPT3001_STC *config)
{
	i2c_write_var(OPT3001_I2C_ADR, STOP, 3, OPT3001_CONFIG_REG, ((config->config & 0xFF00) >> 8), config->config & 0x00FF); //TODO split in high and low byte
	//i2c_write_arr(OPT3001_I2C_ADR, STOP, 2, &(config->config));
	i2c_write_var(OPT3001_I2C_ADR, STOP, 3, OPT3001_L_LIMIT_REG, ((config->low_limit & 0xFF00) >> 8), config->low_limit & 0x00FF); //TODO split in high and low byte
	//i2c_write_arr(OPT3001_I2C_ADR, STOP, 2, &(config->low_limit));
	i2c_write_var(OPT3001_I2C_ADR, STOP, 3, OPT3001_H_LIMIT_REG, ((config->high_limit & 0xFF00) >> 8), config->high_limit & 0x00FF);  //TODO split in high and low byte
	//i2c_write_arr(OPT3001_I2C_ADR, STOP, 2, &(config->high_limit));
	/* Some I2C writes for first configuration on OPT3001 */
	return 0;
}

uint8_t opt3001_get_value(uint32_t *data)
{
	union {
		uint8_t bytes[2];
		uint16_t byte;
	}value;

	uint8_t resp[2];

	i2c_write_var(OPT3001_I2C_ADR, REPT, 1, OPT3001_RESULT_REG);
	i2c_read(OPT3001_I2C_ADR, STOP, 2, resp);

	value.bytes[0] = resp[1];
	value.bytes[1] = resp[0];
	/* Get actual value over I2C and combine higher byte and lower byte */
	*data = ((value.byte & 0x0FFF) << ((value.byte & 0xF000) >> 12));
	return 0;
}
