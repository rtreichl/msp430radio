/*
 * Flash.h
 *
 *  Created on: 27.01.2014
 *      Author: Richi
 */

#ifndef FLASH_H_
#define FLASH_H_

#define FLASH_ADR_START 0x1000
#define FLASH_SEG_SIZE	0x0040
#define FLASH_ADR_STOP	0x10FF

#include <stdint.h>
#include <stdint.h>
#include <string.h>
#include <msp430.h>

uint8_t flash_store(void *data, uint8_t size, uint16_t pos);
uint8_t flash_read(void *data, uint8_t size, uint16_t pos);
void flash_erase(uint8_t *Flash_ptr);

#endif /* FLASH_H_ */
