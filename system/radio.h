/*
 * radio_new.h
 *
 *  Created on: 16.03.2015
 *      Author: Richard
 */

#ifndef RADIO_NEW_H_
#define RADIO_NEW_H_

#include <stdint.h>
#include <driver/pca9530.h>
#include <libs/log_exp_table.h>
#include <driver/si4735.h>
#include <driver/include/si4735_cmd_prop.h>
#include <driver/external_interrupthandler.h>
#include <driver/lcd.h>
#include <driver/flash.h>
#include <driver/timer.h>
#include <settings/radio_configuration.h>
#include <system/time.h>

typedef union radio_stats {
	uint16_t byte;
	uint16_t rds_station_valid:1;
	uint16_t rds_text_valid:1;
	uint16_t rds_time_valid:1;
	uint16_t rds_pty_valid:1;
	uint16_t freq_valid:1;
	uint16_t :11;
} RADIO_STATS;

typedef struct radio {
	uint16_t station_freq;
	int8_t 	station_name[9];
	int8_t	station_text[65];
	int8_t	*station_pty;
	int8_t  volume;
	uint8_t brightness;
	uint8_t contrast;
	TIME_DATE time_date;
} RADIO;

uint8_t radio_brightness(uint8_t brightness);
uint8_t radio_contrast(uint8_t contrast);
uint8_t radio_volume(uint8_t volume);
uint16_t radio_seeking(uint8_t up_down);
uint8_t radio_tune_freq(uint16_t freq);
uint8_t radio_store_station(uint16_t freq, int8_t *name, uint8_t pos);

#endif /* RADIO_NEW_H_ */
