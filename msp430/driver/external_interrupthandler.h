/*
 * external_interrupthandler.h
 *
 *  Created on: 12.03.2015
 *      Author: Richard
 */

#ifndef EXTERNAL_INTERRUPTHANDLER_H_
#define EXTERNAL_INTERRUPTHANDLER_H_

#include <msp430.h>
#include <stdint.h>
#include <settings/radio_pin_mapping.h>

uint8_t ext_interrupt_create(uint8_t int_number, void (*ptrfunction)());
uint8_t ext_interrupt_disable(uint8_t int_number);
uint8_t ext_interrupt_enable(uint8_t int_number);
uint8_t ext_interrupt_delete(uint8_t int_number);


#endif /* EXTERNAL_INTERRUPTHANDLER_H_ */
