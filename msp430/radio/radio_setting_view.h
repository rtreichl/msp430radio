/*
 * radio_setting_view.h
 *
 *  Created on: 01.06.2015
 *      Author: Richard
 */

#ifndef RADIO_RADIO_SETTING_VIEW_H_
#define RADIO_RADIO_SETTING_VIEW_H_

#include <stdint.h>
#include <radio/radio.h>

uint8_t radio_setting_view_text(ENCODER *encoder_left, ENCODER *encoder_right, MENU_STC *menu);
uint8_t radio_setting_view_source(ENCODER *encoder_left, ENCODER *encoder_right, MENU_STC *menu);
uint8_t radio_setting_view_freqency(ENCODER *encoder_left, ENCODER *encoder_right, MENU_STC *menu);
uint8_t radio_setting_view_tatp(ENCODER *encoder_left, ENCODER *encoder_right, MENU_STC *menu);
uint8_t radio_setting_view_equalizer(ENCODER *encoder_left, ENCODER *encoder_right, MENU_STC *menu);
uint8_t radio_setting_view_brightness(ENCODER *encoder_left, ENCODER *encoder_right, MENU_STC *menu);

#endif /* RADIO_RADIO_SETTING_VIEW_H_ */
