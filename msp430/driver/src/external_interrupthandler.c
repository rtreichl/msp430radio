/*
 * external_interrupthandler.c
 *
 *  Created on: 11.03.2015
 *      Author: Richard
 */

#include <driver/external_interrupthandler.h>

#define ENCODER_1
#define ENCODER_2

typedef volatile void (*ptrfunc)();
ptrfunc ext_int_handler[16] = {0};

uint8_t ext_interrupt_create(uint8_t int_number, void (*ptrfunction)())
{
	if(int_number > 15) {
		return 0xFF;
	}
	ext_int_handler[int_number] = (ptrfunc)ptrfunction;

	return 0;
}

uint8_t ext_interrupt_delete(uint8_t int_number)
{
	if(int_number > 15) {
		return 0xFF;
	}
	ext_int_handler[int_number] = 0;

	return 0;
}

uint8_t ext_interrupt_enable(uint8_t int_number)
{
	if(int_number/8) {
		P2IFG &= ~(1 << (int_number % 8));
		P2IE |= (1 << (int_number % 8));
	}
	else {
		P1IFG &= ~(1 << (int_number % 8));
		P1IE |= (1 << (int_number % 8));
	}
	return 0;
}

uint8_t ext_interrupt_disable(uint8_t int_number)
{
	if(int_number/8) {
		P2IE &= ~(1 << int_number % 8);
	}
	else {
		P1IE &= ~(1 << int_number % 8);
	}
	return 0;
}

#pragma vector = PORT1_VECTOR
#pragma vector = PORT2_VECTOR
__interrupt void Port12_interrupts(void)
{
	uint8_t i;
	uint8_t pin;

	if(P1IFG != 0) {
		P1IFG &= P1IE;
		for(i = 0, pin = P1IFG; pin > 0; i++) {
			if(ext_int_handler[i] != 0 && (pin & 0x01)) {
				ext_int_handler[i]();
			}
			pin >>= 1;
		}
		P1IFG = 0;
	}

	if(P2IFG != 0) {
		P2IFG &= P2IE;
		for(i = 8, pin = P2IFG; pin > 0; i++) {
			if(ext_int_handler[i] != 0 && (pin & 0x01)) {
				ext_int_handler[i]();
			}
			pin >>= 1;
		}
		P2IFG = 0;
	}
	_BIC_SR(LPM3_EXIT);
}
