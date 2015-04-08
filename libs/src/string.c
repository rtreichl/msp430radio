/*
 * string.c
 *
 *  Created on: 08.04.2015
 *      Author: Richard
 */
#include <libs/string.h>

uint8_t string_fixedpoint_to_array(char *str, uint16_t freq)
{
	uint8_t count = 0;
	freq /= 10;
	str[0] = ' ';
	str[5] = 0;
	str += 4;
	while (freq > 0) {
		*str = freq % 10 + '0';
		str--;
		freq /= 10;
		if (count++ == 0) {
			*str = '.';
			str--;
		}
	}
	return 0;
}

uint8_t string_hex_to_array(char *str, uint16_t value, uint8_t size)
{
	str[size] = 0;
	str += size - 1;
	if(value == 0) {
		*str = '0';
		str--;
	}
	while (size > 0) {
		if (value != 0) {
			*str = value % 16 + '0';
			if (*str > '9') {
				*str += 'A' - '9' - 1;
			}
			value /= 16;
		}
		else {
			*str = ' ';
		}
		str--;
		size--;

	}
	return 0;
}

uint8_t string_int_to_array(char *str, int16_t value, uint8_t size, uint8_t base)
{
	uint8_t tmp_sign = 0;
	str[size] = 0;
	str += size - 1;
	if(value == 0) {
		*str = '0';
		str--;
	} else if (value < 0) {
		tmp_sign = 1;
		value = (uint16_t)(-1 * value);
	}

	while (size > 0) {
		if (value != 0) {
			*str = value % base + '0';
			if (*str > '9') {
				*str += 'A' - '9' - 1;
			}
			value /= base;
		}
		else {
			if(tmp_sign == 1) {
				*str = '-';
				tmp_sign = 0;
			} else {
				*str = ' ';
			}
		}
		str--;
		size--;

	}
	return 0;
}
