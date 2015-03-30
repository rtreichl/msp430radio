/*
 * Flash.c
 *
 *  Created on: 27.01.2014
 *      Author: Richi
 */

#include <driver/flash.h>


uint8_t store_data_to_flash (int8_t *data, uint8_t size, uint8_t pos)
{
	_DINT();
	uint8_t *Flash_ptr = (uint8_t *)FLASH_ADR_START;
	int8_t seg_store[FLASH_SEG_SIZE];
	uint8_t i = 0;

	if(size + pos > FLASH_SEG_SIZE) {
		return 0xFF;
	}

	switch(pos / FLASH_SEG_SIZE) {
		case 0:
			read_flash(seg_store, (uint8_t)FLASH_SEG_SIZE, 0);
			erase_flash(Flash_ptr);
			break;
		case 1:
			read_flash(seg_store, (uint8_t)FLASH_SEG_SIZE, (uint8_t)FLASH_SEG_SIZE);
			erase_flash(Flash_ptr + (uint8_t)FLASH_SEG_SIZE);
			break;
		case 2:
			read_flash(seg_store, (uint8_t)FLASH_SEG_SIZE, 2 * (uint8_t)FLASH_SEG_SIZE);
			erase_flash(Flash_ptr + 2 * (uint8_t)FLASH_SEG_SIZE);
			break;
		default:
			return 0xFE;
	}

	memcpy(seg_store + pos, data, size);

	FCTL3 = FWKEY;                           	// Clear Lock bit
	FCTL1 = FWKEY + WRT + BLKWRT;            	// Set WRT bit for write operation

	for(i = pos; i < FLASH_SEG_SIZE; i++) {
		Flash_ptr[i] = seg_store[i - pos];		// Write value to flash
		while(BUSY & FCTL3);
	}

	FCTL1 = FWKEY;
	while(BUSY & FCTL3);						// Clear WRT bit
	FCTL3 = FWKEY + LOCK;                    	// Set LOCK bit
	_EINT();

	return 0;
}

uint8_t read_flash(int8_t *data, uint8_t size, uint8_t pos)
{
	memcpy(data, (int8_t *)FLASH_ADR_START + pos, size);
	return 0;
}

void erase_flash(uint8_t *Flash_ptr)
{
	_DINT();
	while(BUSY & FCTL3);
	FCTL1 = FWKEY + ERASE;                   // Set Erase bit
	FCTL3 = FWKEY;                           // Clear Lock bit
	*Flash_ptr = 0;                          // Dummy write to erase Flash segment
	while(BUSY & FCTL3);
	FCTL1 = FWKEY;                           // Clear WRT bit
	FCTL3 = FWKEY + LOCK;                    // Set LOCK bit
	_EINT();
}
