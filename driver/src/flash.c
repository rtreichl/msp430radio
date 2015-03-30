/*
 * Flash.c
 *
 *  Created on: 27.01.2014
 *      Author: Richi
 */

#include "msp430.h"
#include <driver/flash.h>

void stor_data_to_flash (char *Flash_ptr, char *data, char value, unsigned char pos)
{
	_DINT();
	char tmp_stor[150];
	unsigned char i = 0;
	for(i = 0;i<150;i++)
		tmp_stor[i] = Flash_ptr[i];
	erase_flash(Flash_ptr);
	erase_flash(Flash_ptr+64);
	erase_flash(Flash_ptr+128);
	FCTL1 = FWKEY + WRT;                     // Set WRT bit for write operation
	FCTL3 = FWKEY;                           // Clear Lock bit
	for(i = pos;i<9+pos;i++)
			tmp_stor[i] = *data++;
	tmp_stor[i] = value;
	for(i = 0;i<150;i++)
	{
		Flash_ptr[i] = tmp_stor[i];  	                 // Write value to flash
		while(BUSY & FCTL3);
	}
	FCTL1 = FWKEY;                           // Clear WRT bit
	FCTL3 = FWKEY + LOCK;                    // Set LOCK bit
	_EINT();
}

void erase_flash(char *Flash_ptr)
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