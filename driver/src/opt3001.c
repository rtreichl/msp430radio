/*
 * opt3001.c
 *
 *  Created on: 26.03.2015
 *      Author: Richard
 */

#include <driver/opt3001.h>

uint8_t opt3001_init(OPT3001_STC config)
{
	/* Some I2C writes for first configuration on OPT3001 */
	return 0;
}

uint8_t opt3001_get_value(uint32_t *data)
{
	uint16_t value;
	/* Get actual value over I2C and combine higher byte and lower byte */
	//*data = float_to_int(value);
	return 0;
}
