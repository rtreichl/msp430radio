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
#include <driver/timer.h>
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

	void si4735_interrupt(void);

	uint8_t si4735_configure_seeking(uint16_t top_freq, uint16_t bot_freq, uint8_t freq_spacing, uint8_t snr, uint8_t rssi);

	uint8_t si4735_set_property( uint16_t property, uint16_t data);

	uint8_t si4735_fm_seek_start(uint8_t up_down);

	uint8_t si4735_fm_tune_status(uint8_t cancel, uint8_t intack, uint8_t *resp);

	uint8_t si4735_fm_tune_freq(uint16_t frequency);

#endif /* SI4735_H_ */
