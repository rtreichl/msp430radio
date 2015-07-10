/*
 * radio_setting_view.c
 *
 *  Created on: 01.06.2015
 *      Author: Richard
 */

#include <radio/radio_setting_view.h>
//#include <driver/driver.h>

uint8_t radio_setting_view_text(ENCODER *encoder_left, ENCODER *encoder_right, MENU_STC *menu)
{
	int8_t pos = 0;
	pos = radio.settings.display_view - menu->y + 1;
	if(pos >= 0 && pos <= 2) {
		lcd_create_view("\52", 15, pos, 0, 0);
	}
	return 0;
}

uint8_t radio_setting_view_source(ENCODER *encoder_left, ENCODER *encoder_right, MENU_STC *menu)
{
	int8_t pos = 0;
	pos = radio.settings.source - menu->y + 1;
	if(pos >= 0 && pos <= 2) {
		lcd_create_view("\52", 15, pos, 0, 0);
	}
	return 0;
}

uint8_t radio_setting_view_freqency(ENCODER *encoder_left, ENCODER *encoder_right, MENU_STC *menu)
{
	int8_t pos = 0;
	RADIO_SETTINGS *settings = (RADIO_SETTINGS *)RADIO_SETTINGS_STORE_ADR;
	if(settings->frequency < RADIO_FREQENCY_MIN) {
		pos = 0;
	}
	else if(settings->frequency > RADIO_FREQENCY_MAX) {
		pos = 1;
	}
	else {
		pos = 2;
	}
	pos += 1 - menu->y;
	if(pos >= 0 && pos <= 2) {
		lcd_create_view("\52", 15, pos, 0, 0);
	}
	return 0;
}

uint8_t radio_setting_view_tatp(ENCODER *encoder_left, ENCODER *encoder_right, MENU_STC *menu)
{
	int8_t pos = 0;
	pos = radio.settings.ta_tp - menu->y + 1;
	if(pos >= 0 && pos <= 2) {
		lcd_create_view("\52", 15, pos, 0, 0);
	}
	return 0;
}

uint8_t radio_setting_view_equalizer(ENCODER *encoder_left, ENCODER *encoder_right, MENU_STC *menu)
{
	int8_t pos = 0;
	pos = radio.settings.equalizer - menu->y + 1;
	if(pos >= 0 && pos <= 2) {
		lcd_create_view("\52", 15, pos, 0, 0);
	}
	return 0;
}

uint8_t radio_setting_view_brightness(ENCODER *encoder_left, ENCODER *encoder_right, MENU_STC *menu)
{
	int8_t pos = 0;
	pos = radio.settings.backlight - menu->y + 1;
	if(pos >= 0 && pos <= 2) {
		lcd_create_view("\52", 15, pos, 0, 0);
	}
	return 0;
}
