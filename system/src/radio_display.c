/*
 * radio_display.c
 *
 *  Created on: 08.04.2015
 *      Author: Richard
 */

#include <system/radio_display.h>

uint8_t radio_display_speaker()
{
	char tmp_string[9];
	switch (radio.status.audio_status) {
	case AUDIO_MUTE:
		lcd_create_view("\7÷", 0, 1, 0, 0);
		break;
	case AUDIO_VOLUME:
		string_int_to_array(tmp_string, radio.settings.volume, 3, 10);
		lcd_create_view(tmp_string, 1, 1, 0, 0);
		lcd_create_view("%", 4, 1, 0, 0);
		lcd_create_view("\7", 0, 1, 0, 0);
		radio.status.audio_status = 0;
		break;
	default:
		lcd_create_view("\7û", 0, 1, 0, 0);	//Audio normal
		break;
	}
	return 0;
}

uint8_t radio_display_rds()
{
	char tmp_string[9];
	if(radio.status.text_valid == VALID)
	if(radio.status.scroll_text < 15) {
		lcd_create_view(radio.rds.text, 15 - radio.status.scroll_text, 2, radio.status.scroll_text + 1, 0);
	}
	else {
		lcd_create_view(radio.rds.text - 15 + radio.status.scroll_text, 0, 2, 16, 0);
	}
	time_date_to_array(tmp_string);
	lcd_create_view(tmp_string,  8, 1, 0, 0);
	return 0;
}

uint8_t radio_display_pipty()
{
	char tmp_string[9];
	string_hex_to_array(tmp_string, radio.rds.pi, 4);
	lcd_create_view("PI:", 9, 1, 0, 0);
	lcd_create_view(tmp_string, 12, 1, 0, 0);
	lcd_create_view(pty_text[radio.rds.pty], 0, 2, 0, 0);
	return 0;
}

uint8_t radio_display_rsq()
{
	char tmp_string[9];
	string_int_to_array(tmp_string, radio.rsq.rssi, 3, 10);
	lcd_create_view(tmp_string, 9, 1, 0, 0);
	lcd_create_view("dBuV", 12, 1, 0, 0);
	string_int_to_array(tmp_string, radio.rsq.snr, 3, 10);
	lcd_create_view(tmp_string, 0, 2, 0, 0);
	lcd_create_view("dB", 3, 2, 0, 0);
	string_int_to_array(tmp_string, radio.rsq.multi, 3 ,10);
	lcd_create_view(tmp_string, 6, 2, 0, 0);
	lcd_create_view("%", 9, 2, 0, 0);
	string_int_to_array(tmp_string, radio.rsq.freq_off, 3, 10);
	lcd_create_view(tmp_string, 10, 2, 0, 0);
	lcd_create_view("kHz", 13, 2, 0, 0);
	return 0;
}

uint8_t radio_display_handler(uint8_t blend_scroll)
{
	char tmp_string[9];
	switch(radio.settings.display_view) {
	case RADIO_RDS_VIEW:
		radio_display_rds();
		break;
	case RADIO_RSQ_VIEW:
		radio_display_rsq();
		break;
	case RADIO_PIPTY_VIEW:
		radio_display_pipty();
		break;
	}

	if(radio.status.name_valid == VALID) {
		lcd_create_view(radio.rds.name, 0, 0, 0, 0);
	}
	else {
		string_fixedpoint_to_array(tmp_string, radio.settings.frequency);
		lcd_create_view(tmp_string, 0, 0, 0, 0);
		lcd_create_view("MHz", 5, 0, 0, 0); //TODO add this to a String table
	}
	radio_display_speaker();
	if(blend_scroll != 0) {
		menu_scroll(blend_scroll);
	}

	if(radio.rds.tp == 1) {
		if(radio.rds.ta == 1) {
			if(radio.status.scroll_text % 2 == 0) {
				lcd_create_view("\020", 6, 1, 0, 0);
			}
		}
		else {
			lcd_create_view("\020", 6, 1, 0, 0);
		}
	}

	time_time_to_array(tmp_string);
	lcd_create_view("\6", 10, 0, 0, 0); //TODO \6 add this to lcd symbols
	lcd_create_view(tmp_string,  11, 0, 0, 1);

	return 0;
}
