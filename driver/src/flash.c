/*
 * Flash.c
 *
 *  Created on: 27.01.2014
 *      Author: Richi
 */

#include <driver/flash.h>


uint8_t store_data_to_flash (void *data, uint8_t size, uint16_t pos)
{
	__disable_interrupt();
	uint8_t *Flash_ptr = (uint8_t *)FLASH_ADR_START;
	uint8_t seg_store[FLASH_SEG_SIZE];
	register uint8_t i = 0;

	Flash_ptr = (uint8_t *)pos;

	pos -= FLASH_ADR_START;

	switch(pos / FLASH_SEG_SIZE) {
		case 0:
			read_flash(seg_store, (uint8_t)FLASH_SEG_SIZE, 0);
			break;
		case 1:
			read_flash(seg_store, (uint8_t)FLASH_SEG_SIZE, (uint8_t)FLASH_SEG_SIZE);
			break;
		case 2:
			read_flash(seg_store, (uint8_t)FLASH_SEG_SIZE, 2 * (uint8_t)FLASH_SEG_SIZE);
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

uint8_t read_flash(void *data, uint8_t size, uint8_t pos)
{
	memcpy(data, (int8_t *)FLASH_ADR_START + pos, size);
	return 0;
}

void erase_flash(uint8_t *Flash_ptr)
{
	while(BUSY & FCTL3);
	FCTL1 = FWKEY + ERASE;                   // Set Erase bit
	FCTL3 = FWKEY;                           // Clear Lock bit
	*Flash_ptr = 0;                          // Dummy write to erase Flash segment
	while(BUSY & FCTL3);
	FCTL1 = FWKEY;                           // Clear WRT bit
	FCTL3 = FWKEY + LOCK;                    // Set LOCK bit
}
