/*
 * Flash.h
 *
 *  Created on: 27.01.2014
 *      Author: Richi
 */

#ifndef FLASH_H_
#define FLASH_H_

#define FLASH_ADR_STATION_NAME 0x1000
#define FLASH_ADR_STATION_FREQ 0x1009

#include <stdint.h>

void stor_data_to_flash (int8_t *Flash_ptr, int8_t *data, int8_t value, uint8_t pos);

void erase_flash(int8_t *Flash_ptr);

#endif /* FLASH_H_ */
