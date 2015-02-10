/*
 * SI4735.h
 *
 *  Created on: 04.07.2013
 *      Author: Alexander Raskopf
 */

#ifndef SI4735_H_
#define SI4735_H_

#include <stdint.h>
#include <driver/i2c.h>
#include <Timer.h>
#include <settings/radio_pin_mapping.h>


#define I2C_SI4735 0x11
#define SI4735_VOLUME_MAX 63

	void SI4735_Power_Up (void);

	void SI4735_Power_Down (void);

	void SI4735_Refclk_Freq (void);

	void SI4735_RX_Volume (void);

	void SI4735_Fm_Deemphasis (void);

	void SI4735_Rx_Hard_Mute (void);

	void SI4735_FM_RDS_INT_SOURCE();

	void SI4735_FM_RDS_INT_FIFO_COUNT();

	void SI4735_FM_RDS_CONFIG();

	void SI4735_Fm_Seek_Band_Bottom (void);

	void SI4735_Fm_Seek_Band_Top (void);

	void SI4735_Fm_Tune_Freq (void);

	void SI4735_Fm_Tune_Status (void);

	void SI4735_seek_Down(void);

	void SI4735_seek_Up(void);

	void SI4735_INIT(void); //Initalisiert je nach Anforderung den SI4735 Radiochip

	void SI4735_Start_Volume (void);

	void SI4735_Set_Volume (signed char Volume);

	void SI4735_Fm_Tune_Freq_2 (unsigned int Frequenz);


#endif /* SI4735_H_ */
