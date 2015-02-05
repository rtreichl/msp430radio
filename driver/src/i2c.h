/*
 * USCIT_I2C.h
 *
 *  Created on: 03.07.2013
 *      Author: Alexander Raskopf
 */

#ifndef USCIT_I2C_H_
#define USCIT_I2C_H_

#define STOP 0
#define REPT 1

#include <stdint.h>

void USCI_I2C_INIT (uint8_t Teiler);
void USCI_I2C_WRITE2 (uint8_t addr, uint8_t rept_start, uint8_t n_args, ...);
void USCI_I2C_READ (uint8_t addr, uint8_t rept_start, uint8_t RXBytes, uint8_t *RxData);


#endif /* USCIT_I2C_H_ */
