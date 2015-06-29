/*
 * string.h
 *
 *  Created on: 08.04.2015
 *      Author: Richard
 */

#ifndef LIBS_STRING_H_
#define LIBS_STRING_H_

#include <stdint.h>

uint8_t string_fixedpoint_to_array(char *str, uint16_t freq);
uint8_t string_hex_to_array(char *str, uint16_t value, uint8_t size);
uint8_t string_int_to_array(char *str, int16_t value, uint8_t size, uint8_t base);
uint8_t string_fixpoint_to_array(char *str, uint32_t value, uint8_t size, uint8_t fp);


#endif /* LIBS_STRING_H_ */
