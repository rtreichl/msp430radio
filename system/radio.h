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
#include <driver/tpa2016d2.h>
#include <driver/lcd.h>
#include <driver/flash.h>
#include <driver/timer.h>
#include <driver/encoder.h>
#include <menu/lang/language.h>
#include <settings/radio_configuration.h>
#include <system/time.h>

#define RADIO_RDS_VIEW 0
#define RADIO_RSQ_VIEW 1
#define RADIO_PIPTY_VIEW 2
#define VALID 1
#define NOT_VALID 0
#define SOURCE_FM 0
#define SOURCE_AM 1
#define SOURCE_LINEIN 2

#define Start_up_1 		"HS-Rosenheim"
#define	Shift_left_1	2
#define	Start_up_2		"FM/AM  Radio"
#define	Shift_left_2	2
#define Start_up_3		"Version 1.00"
#define	Shift_left_3	2

#define AMPLIFIER_GAIN 15

#define BUTTON_PRESS_LONG 'l'
#define BUTTON_PRESS_SHORT 'k'
#define BUTTON_FREE 'f'

typedef struct radio_status {
	uint16_t station_valid:1;
	uint16_t text_valid:1;
	uint16_t time_valid:1;
	uint16_t name_valid:1;
	uint16_t freq_valid:1;
	uint16_t scroll_text:4;
	uint16_t display_mode:2;
	uint16_t audio_mute:1;
	uint16_t :4;
	uint16_t source_select:2;
} RADIO_STATUS;

typedef struct radio_rsq {
	uint8_t rssi;
	uint8_t snr;
	uint8_t multi;
	int8_t freq_off;
} RADIO_RSQ;

typedef struct radio_rds {
	char 	name[9];
	char	text[65];
	uint8_t pty;
	uint16_t pi;
} RADIO_RDS;

typedef struct radio {
	uint16_t station_freq;
	int8_t  volume;
	uint8_t brightness;
	uint8_t contrast;
	RADIO_STATUS status;
	RADIO_RDS rds;
	RADIO_RSQ rsq;
} RADIO;

uint8_t radio_brightness(uint8_t brightness);
uint8_t radio_contrast(uint8_t contrast);
uint8_t radio_volume(int8_t *volume);
uint8_t radio_display_handler(uint8_t blend_scroll);
uint16_t radio_seeking(uint8_t up_down);
uint8_t radio_main(uint8_t *encoder_left_button, int8_t *encoder_left_count, uint8_t *encoder_right_button, int8_t *encoder_right_count);
uint8_t radio_init();
uint8_t radio_tune_freq(uint16_t freq);
uint8_t radio_freq_to_string(char *str, uint16_t freq);
uint8_t radio_value_to_string(char *str, int16_t value, uint8_t size, uint8_t base);
uint8_t radio_store_station(uint16_t freq, int8_t *name, uint8_t pos);

#endif /* RADIO_NEW_H_ */
