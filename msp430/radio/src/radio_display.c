/*
 * radio_display.c
 *
 *  Created on: 08.04.2015
 *      Author: Richard
 */

#include <radio/radio_display.h>
#include <radio/radio_settings.h>
#include <libs/string.h>
#include <system/time.h>

//----------------------------------------------------------------------------------------
//
/// \brief Updates the speaker symbol according to the current audio state
//
/// \param	void
//
/// \retval uint8_t
//
/// \remarks
//
//----------------------------------------------------------------------------------------

uint8_t radio_display_speaker()
{
	char tmp_string[9];
	lcd_create_view(symbol_speaker_text, 0, 1, 0, 0);
	switch (radio.status.audio_status) {
	case RADIO_AUDIO_MUTE:
		lcd_create_view(symbol_mute_text, 1, 1, 0, 0);
		break;
	case RADIO_AUDIO_SCROLL:
		itoa(radio.settings.volume, tmp_string, 10, 3);
		lcd_create_view(tmp_string, 1, 1, 0, 0);
		lcd_create_view(unit_percentage_text, 4, 1, 0, 0);
		radio.status.audio_status = RADIO_AUDIO_NORMAL;
		break;
	case RADIO_AUDIO_NORMAL:
		lcd_create_view(symbol_ouput_text, 1, 1, 0, 0);
		break;
	}
	return 0;
}

//----------------------------------------------------------------------------------------
//
/// \brief Displays the RDS data
//
/// \param	void
//
/// \retval uint8_t
//
/// \remarks
//
//----------------------------------------------------------------------------------------
uint8_t radio_display_rds()
{
	static char rds_text[65] = {0};
	static uint8_t rds_end = 64;
	char tmp_string[9];
	uint8_t i = 0;

	if(radio.status.text_valid == RADIO_VALID) {

		if(radio.status.scroll_text >= rds_end + RADIO_DISPLAY_WIDTH){
			radio.status.scroll_text = 0;
		}

		if(radio.status.scroll_text == 0) {
			memcpy(rds_text, radio.rds.text, 64);
			for(i = 0; i <= 64; i++) {
				if(rds_text[i] == 0 || rds_text[i] == 13) {
					rds_end = i - 1;
					break;
				}
			}
		}

		if(radio.status.scroll_text < RADIO_DISPLAY_WIDTH - 1) {
			lcd_create_view(rds_text, RADIO_DISPLAY_WIDTH - 1 - radio.status.scroll_text, 2, radio.status.scroll_text + 2, 0);
		}
		if(radio.status.scroll_text >= RADIO_DISPLAY_WIDTH - 1 && radio.status.scroll_text < rds_end) {
			lcd_create_view(rds_text - 15 + radio.status.scroll_text, 0, 2, 16, 0);
		}
		if(radio.status.scroll_text >= rds_end){
			if(RADIO_DISPLAY_WIDTH - (radio.status.scroll_text - rds_end) > 1)
				lcd_create_view(rds_text - (RADIO_DISPLAY_WIDTH - 1) + radio.status.scroll_text, 0, 2, RADIO_DISPLAY_WIDTH - (radio.status.scroll_text - rds_end), 0);
			else
				lcd_create_view(*(rds_text - (RADIO_DISPLAY_WIDTH - 1) + radio.status.scroll_text), 0, 2, 1, 0);
		}


	}
	else {
		radio.status.scroll_text = 200;
	}

	time_date_to_array(tmp_string);
	lcd_create_view(tmp_string,  8, 1, 0, 0);
	return 0;
}

//----------------------------------------------------------------------------------------
//
/// \brief Displays the RDS data
//
/// \param	void
//
/// \retval uint8_t
//
/// \remarks
//
//----------------------------------------------------------------------------------------

uint8_t radio_display_pipty()
{
	char tmp_string[9];
	itoa(radio.rds.pi, tmp_string, 16, 4);
	lcd_create_view(symbol_PI_text, 9, 1, 0, 0);
	lcd_create_view(tmp_string, 12, 1, 0, 0);
	lcd_create_view(pty_text[radio.rds.pty], 0, 2, 0, 0);
	return 0;
}

//----------------------------------------------------------------------------------------
//
/// \brief Display the PTY/PI states
//
/// \param void
//
/// \retval uint8_t
//
/// \remarks
//
//----------------------------------------------------------------------------------------

uint8_t radio_display_brightness()
{
	char tmp_string[11];
	BRIGHTNESS tmp_bright = radio_brightness(1);
	string_fixpoint_to_array(tmp_string, (*(tmp_bright.sensor) >> 4), 7, 2);
	lcd_create_view(tmp_string, 6, 2, 0, 0);
	lcd_create_view(unit_lux_text, 13, 2, 0, 0);
	itoa(tmp_bright.display, tmp_string, 10, 3);
	lcd_create_view(tmp_string, 0, 2, 0, 0);
	lcd_create_view(unit_percentage_text, 3, 2, 0, 0);
	return 0;
}

//----------------------------------------------------------------------------------------
//
/// \brief Display the RSQ
//
/// \param void
//
/// \retval uint8_t
//
/// \remarks
//
//----------------------------------------------------------------------------------------

uint8_t radio_display_rsq()
{
	char tmp_string[9];
	itoa(radio.rsq.rssi, tmp_string, 10, 3);
	lcd_create_view(tmp_string, 9, 1, 0, 0);
	lcd_create_view(unit_dBuV_text, 12, 1, 0, 0);
	itoa(radio.rsq.snr, tmp_string, 10, 3);
	lcd_create_view(tmp_string, 0, 2, 0, 0);
	lcd_create_view(unit_dB_text, 3, 2, 0, 0);
	itoa(radio.rsq.multi, tmp_string, 10, 3);
	lcd_create_view(tmp_string, 6, 2, 0, 0);
	lcd_create_view(unit_percentage_text, 9, 2, 0, 0);
	itoa(radio.rsq.freq_off, tmp_string, 10, 3);
	lcd_create_view(tmp_string, 10, 2, 0, 0);
	lcd_create_view(unit_kHz_text, 13, 2, 0, 0);
	return 0;
}

//----------------------------------------------------------------------------------------
//
/// \brief Handler for the display
//
/// Updates the display according to the selected state in the settings menu\n
//
/// \param	<blend_scroll>	[in]	Identifies if to blend scrolling
/// \param	<value>			[in]	the value to be scrolled
//
/// \retval uint8_t
//
/// \remarks
//
//----------------------------------------------------------------------------------------

uint8_t radio_display_handler(uint8_t blend_scroll, uint8_t value)
{
	char tmp_string[9];
	switch(radio.settings.display_view) {
	case RADIO_SETTINGS_RDS_VIEW:
		radio_display_rds();
		break;
	case RADIO_SETTINGS_RSQ_VIEW:
		radio_display_rsq();
		break;
	case RADIO_SETTINGS_PIPTY_VIEW:
		radio_display_pipty();
		break;
	case RADIO_SETTINGS_BRIGTH_VIEW:
		radio_display_brightness();
		break;
	}

	if(radio.status.name_valid == RADIO_VALID) {
		lcd_create_view(radio.rds.name, 0, 0, 0, 0);
	}
	else {
		string_fixpoint_to_array(tmp_string, radio.settings.frequency / 10, 5, 1);
		lcd_create_view(tmp_string, 0, 0, 0, 0);
		lcd_create_view(unit_MHz_text, 5, 0, 0, 0);
	}
	radio_display_speaker();
	if(blend_scroll != 0) {
		menu_scroll(value);
	}

	if(radio.settings.ta_tp == RADIO_SETTINGS_TA_TP_ON) {
		if(radio.status.volume_ta == 1) {
			if(radio.status.scroll_text % 2 == 0) {
				lcd_create_view(symbol_TM_text, 6, 1, 0, 0);
			}
		}
		else {
			lcd_create_view(symbol_TM_text, 6, 1, 0, 0);
		}
	}

	time_time_to_array(tmp_string);
	lcd_create_view(symbol_clock_text, 10, 0, 0, 0);
	lcd_create_view(tmp_string,  11, 0, 0, 1);

	return 0;
}
