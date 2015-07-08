/*
 * string.c
 *
 *  Created on: 08.04.2015
 *      Author: Richard
 */
#include <libs/string.h>

uint8_t string_fixpoint_to_array(char *str, uint32_t value, uint8_t size, uint8_t fp)
{
	str[size] = 0;
	str += size - 1;
	while (size--) {
		if(value != 0) {
			*str = value % 10 + '0';
			value /= 10;
		}
		else {
			*str = ' ';
		}
		if (--fp == 0) {
			str--;
			*str = '.';

		}
		str--;
	}
	return 0;
}

uint8_t itoa(int16_t num, char *str, uint8_t base, uint8_t size)
{
	uint8_t sign = 0;
	int8_t i = size;

	str[i--] = 0;

	if(num == 0) {
		str[i--] = '0';
	}

	if(num < 0 && base == 10) {
		sign = 1;
		num = -num;
	}

	while(num != 0) {
		str[i] = (uint16_t)num % base + '0';
		if (str[i] > '9') {
			str[i] += 'A' - '9' - 1;
		}
		num = (uint16_t)num / base;
		i--;
	}

	if(sign == 1) {
		str[i--] = '-';
	}



	while(i > -1){
		str[i--] = ' ';
	}

	return 0;
}
