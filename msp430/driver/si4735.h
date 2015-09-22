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

uint8_t si4735_init(uint8_t volume, uint16_t freq); //Initalisiert je nach Anforderung den SI4735 Radiochip
void si4735_interrupt(void);
uint8_t si4735_power_down(void);
uint8_t si4735_power_up(uint8_t mode);
uint8_t si4735_configure_seeking(uint16_t top_freq, uint16_t bot_freq, uint8_t freq_spacing, uint8_t snr, uint8_t rssi);
uint8_t si4735_set_property( uint16_t property, uint16_t data);
SI4735_FM_STATUS si4735_fm_seek_start(uint8_t up_down);
uint8_t si4735_fm_tune_status(uint8_t cancel, uint8_t intack, uint8_t *resp);
SI4735_FM_STATUS si4735_fm_tune_freq(uint16_t frequency);
uint8_t si4735_fm_rsq_status(uint8_t intack, uint8_t *resp);

#endif /* SI4735_H_ */
