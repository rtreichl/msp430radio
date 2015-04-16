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
#include <menu/menu.h>
#include <settings/radio_configuration.h>
#include <system/station_list.h>
#include <radio/radio_display.h>
#include <radio/radio_settings.h>

#include <libs/string.h>
#include <driver/opt3001.h>
//#include <system/rds.h>
#include <system/time.h>

#define RADIO_RDS_VIEW 0
#define RADIO_RSQ_VIEW 1
#define RADIO_PIPTY_VIEW 2
#define VALID 1
#define NOT_VALID 0
#define SOURCE_FM 0
#define SOURCE_AM 1
#define SOURCE_LINEIN 2

#define RADIO_STORE_VALUE 1

#define AUDIO_NORMAL	0
#define AUDIO_MUTE		1
#define AUDIO_VOLUME	2

typedef struct radio_status {
	uint8_t station_valid:1;
	uint8_t text_valid:1;
	uint8_t time_valid:1;
	uint8_t audio_status:2;
	uint8_t freq_valid:1;
	uint8_t name_valid:1;
	uint8_t volume_ta:1;
	uint8_t scroll_text;
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
	uint16_t pi;
	uint16_t pty:5;
	uint16_t ta:1;
	uint16_t tp:1;
	uint16_t ms:1;
	uint16_t di:4;
} RADIO_RDS;

typedef struct radio_settings {
	uint32_t display_view:2;
	uint32_t contrast:4;
	uint32_t brightness:6;
	uint32_t ta_tp:1;
	uint32_t equalizer:3;
	uint32_t source:2;
	uint32_t volume:7;
	uint32_t volume_ta:7;
	uint16_t frequency;
} RADIO_SETTINGS;

typedef struct radio {
	RADIO_STATUS status;
	RADIO_RDS rds;
	RADIO_RSQ rsq;
	RADIO_SETTINGS settings;
} RADIO;

extern RADIO radio;

uint8_t radio_volume(int8_t volume);
uint8_t radio_settings(uint8_t *encoder_right_button, int8_t *encoder_right_count, uint8_t entry_num);
uint16_t radio_seeking(uint8_t up_down);
uint8_t radio_main(uint8_t *encoder_left_button, int8_t *encoder_left_count, uint8_t *encoder_right_button, int8_t *encoder_right_count, uint8_t entry_num);
uint8_t radio_init();
uint8_t radio_tune_freq(uint16_t freq);
uint8_t radio_store_station(uint16_t *freq, char *name, uint8_t pos);
uint8_t radio_auto_search();
uint8_t radio_store_settings(uint8_t freq, uint8_t volume);
uint8_t radio_load_settings();
uint8_t radio_factory_state();

#endif /* RADIO_NEW_H_ */
