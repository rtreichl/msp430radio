/*
 * Flash.c
 *
 *  Created on: 27.01.2014
 *      Author: Richi
 */

#include <driver/flash.h>


uint8_t flash_store(void *data, uint8_t size, uint16_t pos)
{
	__disable_interrupt();
	uint8_t *Flash_ptr = (uint8_t *)FLASH_ADR_START;
	uint8_t seg_store[FLASH_SEG_SIZE];
	register uint8_t i = 0;

	Flash_ptr = (uint8_t *)pos;

	pos -= FLASH_ADR_START;

	switch(pos / FLASH_SEG_SIZE) {
		case 0:
			flash_read(seg_store, (uint8_t)FLASH_SEG_SIZE, FLASH_ADR_START);
			break;
		case 1:
			flash_read(seg_store, (uint8_t)FLASH_SEG_SIZE, FLASH_ADR_START + FLASH_SEG_SIZE);
			break;
		case 2:
			flash_read(seg_store, (uint8_t)FLASH_SEG_SIZE, FLASH_ADR_START + 2 * FLASH_SEG_SIZE);
			break;
		default:
			return 0xFE;
	}

	pos %= FLASH_SEG_SIZE;
	if(size + pos > FLASH_SEG_SIZE) {
		return 0xFF;
	}

	memcpy(seg_store + pos, data, size);

	FCTL1 = FWKEY + ERASE; // Set Erase bit
	FCTL3 = FWKEY; // Clear Lock bit
	*Flash_ptr = 0; // Dummy write to erase Flash segment
	Flash_ptr -= pos;

	FCTL1 = FWKEY + WRT;            	// Set WRT bit for write operation
	//FCTL3 = FWKEY;                           	// Clear Lock bit

	for(i = 0; i < (uint8_t)FLASH_SEG_SIZE; i++) {
		Flash_ptr[i] = seg_store[i];		// Write value to flash
	}

	FCTL1 = FWKEY;					// Clear WRT bit
	FCTL3 = FWKEY + LOCK;                    	// Set LOCK bit

	__enable_interrupt();

	return 0;
}

uint8_t flash_read(void *data, uint8_t size, uint16_t pos)
{
	uint8_t *Flash_ptr = 0;
	Flash_ptr = (uint8_t *)pos;
	memcpy(data, Flash_ptr, size);
	return 0;
}

void flash_erase(uint8_t *Flash_ptr)
{
	while(BUSY & FCTL3);
	FCTL1 = FWKEY + ERASE;                   // Set Erase bit
	FCTL3 = FWKEY;                           // Clear Lock bit
	*Flash_ptr = 0;                          // Dummy write to erase Flash segment
	while(BUSY & FCTL3);
	FCTL1 = FWKEY;                           // Clear WRT bit
	FCTL3 = FWKEY + LOCK;                    // Set LOCK bit
}
