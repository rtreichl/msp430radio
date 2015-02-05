/*
 * radio_new.c
 *
 *  Created on: 31.01.2015
 *      Author: Richard
 */

#include <stdint.h>
#include "pca9530.h"
#include "log_exp_table.h"

uint8_t radio_set_brightness(uint8_t brightness)
{
	pca9530_set_pwm(255-brightnes);

	return 0;
}
