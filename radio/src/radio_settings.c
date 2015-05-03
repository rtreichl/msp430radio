/*
 * radio_settings.c
 *
 *  Created on: 14.04.2015
 *      Author: Richard
 */

#include <radio/radio_settings.h>

uint8_t radio_settings_brightness(uint8_t *encoder_left_button, int8_t *encoder_left_count, uint8_t *encoder_right_button, int8_t *encoder_right_count, uint8_t entry_num)
{
	if(*encoder_right_count != 0) {
		if(*encoder_right_count > 0 && radio.settings.brightness < RADIO_BRIGHTNESS_MAX) {
			radio.settings.brightness += RADIO_BRIGHTNESS_STEP;
		}
		else if(*encoder_right_count < 0 && radio.settings.brightness > RADIO_BRIGHTNESS_MIN){
			radio.settings.brightness -= RADIO_BRIGHTNESS_STEP;
		}
		radio_brightness(radio.settings.brightness);
		*encoder_right_count = 0;
	}
	if(*encoder_right_button == BUTTON_SHORT) {
		*encoder_right_button = BUTTON_FREE;
		radio_store_settings(0, 0);
		return SHORT_UP_TO_CHILD;
	}
	else {
		menu_scroll_settings(radio.settings.brightness * 2);
	}
	return STAY_ON_MENU_POINT;
}

uint8_t radio_settings_contrast(uint8_t *encoder_left_button, int8_t *encoder_left_count, uint8_t *encoder_right_button, int8_t *encoder_right_count, uint8_t entry_num)
{
	if(*encoder_right_count != 0) {
		if(*encoder_right_count > 0 && radio.settings.contrast < RADIO_CONTRAST_MAX) {
			radio.settings.contrast += RADIO_CONTRAST_STEP;
		}
		else if(*encoder_right_count < 0 && radio.settings.contrast > RADIO_CONTRAST_MIN) {
			radio.settings.contrast -= RADIO_CONTRAST_STEP;
		}
		lcd_contrast(radio.settings.contrast / RADIO_CONTRAST_STEP);
		*encoder_right_count = 0;
	}
	if(*encoder_right_button == BUTTON_SHORT) {
		*encoder_right_button = BUTTON_FREE;
		radio_store_settings(0, 0);
		return SHORT_UP_TO_CHILD;
	}
	else {
		menu_scroll_settings(radio.settings.contrast * 10);
	}
	return STAY_ON_MENU_POINT;
}

uint8_t radio_settings_source(uint8_t *encoder_left_button, int8_t *encoder_left_count, uint8_t *encoder_right_button, int8_t *encoder_right_count, uint8_t entry_num)
{
	AUDIO_SW_LINE_DIR |= AUDIO_SW_LINE_PIN;
	AUDIO_SW_GND_DIR |= AUDIO_SW_GND_PIN;

	switch(entry_num) {
	default:
	case SOURCE_FM_ENTRY:
		radio.settings.source = SOURCE_FM;
		AUDIO_SW_LINE_OUT &= ~AUDIO_SW_LINE_PIN;
		AUDIO_SW_GND_OUT &= ~AUDIO_SW_GND_PIN;
		//TODO Implement switch si4735 to FM only when AM mode is implemented else just switch audio switch
		break;
	case SOURCE_AM_ENTRY:
		radio.settings.source = SOURCE_AM;
		AUDIO_SW_LINE_OUT &= ~AUDIO_SW_LINE_PIN;
		AUDIO_SW_GND_OUT &= ~AUDIO_SW_GND_PIN;
		//TODO Implement switch si4735 to AM only when AM mode is implemented else just do nothing else
		break;
	case SOURCE_LINEIN_ENTRY:
		radio.settings.source = SOURCE_LINEIN;
		AUDIO_SW_LINE_OUT |= AUDIO_SW_LINE_PIN;
		AUDIO_SW_GND_OUT |= AUDIO_SW_GND_PIN;
		//TODO set si4735 in powerdown modus only if ta/tp mode is off else poll flag and swith to si4735 for duration ta flag is set
	}
	radio_store_settings(0, 0);
	return SHORT_UP_TO_PARENT;
}

uint8_t radio_settings_equalizer(uint8_t *encoder_left_button, int8_t *encoder_left_count, uint8_t *encoder_right_button, int8_t *encoder_right_count, uint8_t entry_num)
{
	switch(entry_num) {
	case AUDIO_ROCK_ENTRY:
		radio.settings.equalizer = ROCK;
		break;
	case AUDIO_POP_ENTRY:
		radio.settings.equalizer = POP;
		break;
	case AUDIO_HIPHOP_ENTRY:
		radio.settings.equalizer = RAP_HIP_HOP;
		break;
	case AUDIO_NEWS_ENTRY:
		radio.settings.equalizer = NEWS_VOICE;
		break;
	case AUDIO_CLASSIC_ENTRY:
		radio.settings.equalizer = CLASSIC;;
		break;
	case AUDIO_JAZZ_ENTRY:
		radio.settings.equalizer = JAZZ;
		break;
	}
	tpa2016d2_equalizer_mode(radio.settings.equalizer, RADIO_AMPLIFIER_GAIN);
	radio_store_settings(0, 0);
	return SHORT_UP_TO_PARENT;
}

uint8_t radio_settings_volume(uint8_t *encoder_left_button, int8_t *encoder_left_count, uint8_t *encoder_right_button, int8_t *encoder_right_count, uint8_t entry_num)
{
	static int8_t tmp_volume = 0;
	if(tmp_volume == 0 && entry_num != 0) {
		tmp_volume = radio.settings.volume;
		if(entry_num == MENU_VOL_TA_ENTRY) {
			radio.settings.volume = radio.settings.volume_ta;
		}
		if(entry_num == MENU_VOL_START_ENTRY) {
			RADIO_SETTINGS tmp_settings;
			flash_read(&tmp_settings, sizeof(tmp_settings), RADIO_SETTINGS_STORE_ADR);
			radio.settings.volume  = tmp_settings.volume;
		}
		radio_volume(radio.settings.volume);
	}
	if(*encoder_right_count != 0) {
		if(*encoder_right_count > 0 && radio.settings.volume < RADIO_VOLUME_MAX) {
			radio.settings.volume += RADIO_VOLUME_STEP;
		}
		else if(*encoder_right_count < 0 && radio.settings.volume > RADIO_VOLUME_MIN) {
			radio.settings.volume -= RADIO_VOLUME_STEP;
		}
		radio_volume(radio.settings.volume);
		*encoder_right_count = 0;
	}
	if(*encoder_right_button == BUTTON_SHORT) {
		*encoder_right_button = BUTTON_FREE;
		if(tmp_volume != 0) {
			if(entry_num == MENU_VOL_START_ENTRY) {
				radio_store_settings(0, 1);
			}
			else if(entry_num == MENU_VOL_TA_ENTRY) {
				radio.settings.volume_ta = radio.settings.volume;
				radio_store_settings(0, 0);
			}
			radio.settings.volume = tmp_volume;
			tmp_volume = 0;
			radio_volume(radio.settings.volume);
		}
		return SHORT_UP_TO_CHILD;
	}
	else {
		menu_scroll_settings(radio.settings.volume);
	}
	return STAY_ON_MENU_POINT;
}

uint8_t radio_settings_frequency(uint8_t *encoder_left_button, int8_t *encoder_left_count, uint8_t *encoder_right_button, int8_t *encoder_right_count, uint8_t entry_num)
{
	uint16_t tmp_freq = 0;
	uint8_t ret = 0;
	tmp_freq = radio.settings.frequency;
	switch(entry_num) {
	case MENU_FREQ_SEEK_UP_ENTRY:
		radio.settings.frequency = 0;
		radio_store_settings(1, 0);
		radio.settings.frequency = tmp_freq;
		return SHORT_UP_TO_PARENT;
	case MENU_FREQ_SEEK_DOWN_ENTRY:
		radio.settings.frequency = 0xFFFF;
		radio_store_settings(1, 0);
		radio.settings.frequency = tmp_freq;
		return SHORT_UP_TO_PARENT;
	case MENU_FREQ_CHOOSE_ENTRY:
		ret = station_list_handler(encoder_left_button, encoder_left_count, encoder_right_button, encoder_right_count,  entry_num);
		radio.settings.frequency = tmp_freq;
		return ret;
	}
	return SHORT_UP_TO_CHILD;
}

uint8_t radio_settings_view(uint8_t *encoder_left_button, int8_t *encoder_left_count, uint8_t *encoder_right_button, int8_t *encoder_right_count, uint8_t entry_num)
{
	switch(entry_num) {
	case RADIO_RDS_VIEW_ENTRY:
		radio.settings.display_view = RADIO_RDS_VIEW;
		radio_store_settings(0, 0);
		return SHORT_UP_TO_PARENT;
	case RADIO_RSQ_VIEW_ENTRY:
		radio.settings.display_view = RADIO_RSQ_VIEW;
		radio_store_settings(0, 0);
		return SHORT_UP_TO_PARENT;
	case RADIO_PIPTY_VIEW_ENTRY:
		radio.settings.display_view = RADIO_PIPTY_VIEW;
		radio_store_settings(0, 0);
		return SHORT_UP_TO_PARENT;
	}
	return SHORT_UP_TO_CHILD;
}

uint8_t radio_settings_station(uint8_t *encoder_left_button, int8_t *encoder_left_count, uint8_t *encoder_right_button, int8_t *encoder_right_count, uint8_t entry_num)
{
	switch(entry_num) {
	case SEEKING_UP:
		radio.settings.frequency = radio_seeking(1);
		return SHORT_UP_TO_PARENT;
	case SEEKING_DOWN:
		radio.settings.frequency = radio_seeking(0);
		return SHORT_UP_TO_PARENT;
	case STORE_STATION:
	case STATION_VIEW:
		return station_list_handler(encoder_left_button, encoder_left_count, encoder_right_button, encoder_right_count,  entry_num);
	case AUTO_SEARCH:
		radio_auto_search();
		return SHORT_UP_TO_PARENT;
	}
	return SHORT_UP_TO_CHILD;
}

uint8_t radio_settings_tatp(uint8_t *encoder_left_button, int8_t *encoder_left_count, uint8_t *encoder_right_button, int8_t *encoder_right_count, uint8_t entry_num)
{
	switch(entry_num) {
	case MENU_TA_TP_ON:
		radio.settings.ta_tp = 1;
		radio_store_settings(0, 0);
		return SHORT_UP_TO_PARENT;
	case MENU_TA_TP_OFF:
		radio.settings.ta_tp = 0;
		radio_store_settings(0, 0);
		return SHORT_UP_TO_PARENT;
	}
	return SHORT_UP_TO_CHILD;
}

uint8_t radio_settings_reset(uint8_t *encoder_left_button, int8_t *encoder_left_count, uint8_t *encoder_right_button, int8_t *encoder_right_count, uint8_t entry_num)
{
	switch(entry_num) {
	case MENU_RESET_NO:
		return SHORT_UP_TO_PARENT;
	case MENU_RESET_YES:
		radio_factory_state();
		return SHORT_UP_TO_PARENT;
	}
	return SHORT_UP_TO_CHILD;
}
