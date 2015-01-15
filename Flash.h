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


void stor_data_to_flash (char *Flash_ptr, char *data, char value, unsigned char pos);

void erase_flash(char *Flash_ptr);

#endif /* FLASH_H_ */
