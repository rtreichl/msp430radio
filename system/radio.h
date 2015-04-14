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
#include <system/radio_display.h>
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

#define AMPLIFIER_GAIN 3

#define RADIO_BRIGHTNESS_MAX 50
#define RADIO_BRIGHTNESS_MIN 0
#define RADIO_BRIGHTNESS_STEP 1

#define RADIO_CONTRAST_MAX 10
#define RADIO_CONTRAST_MIN 0
#define RADIO_CONTRAST_STEP 1

#define RADIO_VOLUME_MAX 100
#define RADIO_VOLUME_MIN 0
#define RADIO_VOLUME_STEP 2

#define BUTTON_PRESS_LONG 'l'
#define BUTTON_PRESS_SHORT 'k'
#define BUTTON_FREE 'f'

#define AUDIO_MUTE 1
#define AUDIO_VOLUME 2

#define RADIO_STATION_NAME_STORE_ADR 0x1000
#define RADIO_STATION_NAME_STORE_SIZE 8
#define RADIO_STATION_FREQ_STORE_ADR 0x1070
#define RADIO_STATION_FREQ_STORE_SIZE 2

#define RADIO_SETTINGS_STORE_ADR 0x108C

#define RADIO_INIT_STORE_ADR 0x10B8
/*#define RADIO_CONTRAST_STORE_ADR 0x108D
#define RADIO_CONTRAST_STORE_SIZE 1
#define RADIO_VIEW_STORE_ADR 0x108E
#define RADIO_VIEW_STORE_SIZE 1
#define RADIO_SOURCE_STORE_ADR	0x108F
#define RADIO_SOURCE_STORE_SIZE 1
#define RADIO_EQUALIZER_STORE_ADR 0x1090
#define RADIO_EQUALIZER_STORE_SIZE 1
#define RADIO_VOLUME_STORE_ADR 0x1091
#define RADIO_VOLUME_STORE_SIZE 1
#define RADIO_TA_STORE_ADR 0X1092
#define RADIO_TA_STORE_SIZE 1
#define RADIO_FREQENCY_STORE_ADR 0X1093
#define RADIO_FREQENCY_STORE_SIZE 2*/

//TODO add a setting area for store

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

uint8_t radio_source_select(uint8_t entry_num);
uint8_t radio_brightness(uint8_t *encoder_right_button, int8_t *encoder_right_count);
uint8_t radio_contrast(uint8_t *encoder_right_button, int8_t *encoder_right_count);
uint8_t radio_set_volume(int8_t volume);
uint8_t radio_settings(uint8_t *encoder_right_button, int8_t *encoder_right_count, uint8_t entry_num);
uint8_t radio_volume(uint8_t *encoder_right_button, int8_t *encoder_right_count, uint8_t setting);
uint16_t radio_seeking(uint8_t up_down);
uint8_t radio_main(uint8_t *encoder_left_button, int8_t *encoder_left_count, uint8_t *encoder_right_button, int8_t *encoder_right_count);
uint8_t radio_init();
uint8_t radio_tune_freq(uint16_t freq);
uint8_t radio_store_station(uint16_t *freq, char *name, uint8_t pos);
uint8_t radio_auto_search();
uint8_t radio_store_settings(uint8_t freq, uint8_t volume);
uint8_t radio_load_settings();
uint8_t radio_factory_state();
uint8_t radio_frequency(uint8_t *encoder_right_button, int8_t *encoder_right_count, uint8_t entry_num);

#endif /* RADIO_NEW_H_ */
