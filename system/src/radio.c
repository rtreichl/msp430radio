/*
 * radio_new.c
 *
 *  Created on: 31.01.2015
 *      Author: Richard
 */

#include <system/radio.h>

RADIO radio;

uint8_t radio_init()
{
	//load stored values from flash
	//init lcd
	//show start screen
	//init si
	//init amp
	//init background lighting
	//init encoder
	//init time
	return 0;
}

uint8_t radio_brightness(uint8_t brightness)
{
	if(brightness > 100) {
		return 0xFF;
	}
	pca9530_set_pwm(PWM_0, 256-exp_table[brightness]);
	//pca9530_set_pwm(PWM_0,brightness);
	return 0;
}

uint8_t radio_contrast(uint8_t contrast)
{
	if(contrast > 100) {
		return 0xFF;
	}
	lcd_contrast((contrast * 32) / 100);
	return 0;
}

uint8_t radio_volume(uint8_t volume)
{
	if(volume > 100) {
		return 0xFF;
	}
	//volume = ((uint16_t)volume * SI4735_VOLUME_MAX) / 100;
	si4735_set_property(RX_VOLUME, volume);
	_delay_ms(10);
	return 0;
}

uint8_t radio_main(uint8_t *encoder_left_button, int8_t *encoder_left_count)
{
	static uint8_t volume = 0;
	if(*encoder_left_count != 0) {
		if(*encoder_left_count < 0 && radio.volume > 0) {
			radio.volume--;
			*encoder_left_count = 0;
		}
		else if(*encoder_left_count > 0 && radio.volume < 100) {
			radio.volume++;
			*encoder_left_count = 0;
		}
	}

	if(*encoder_right_count != 0) {
		if(*encoder_right_count < 0 && radio.station_freq > 0) {
			radio.station_freq--;
			*encoder_right_count = 0;
		}
		else if(*encoder_right_count > 0 && radio.station_freq < 100) {
			radio.station_freq++;
			*encoder_right_count = 0;
		}
	}
	if(*encoder_left_button == BUTTON_PRESS_SHORT)
	{
		*encoder_left_button = BUTTON_PRESS_FREE;
		return 0xFF;
	}
	if(*encoder_left_button == BUTTON_PRESS_LONG)
	{
		*encoder_left_button = BUTTON_PRESS_FREE;
		return 0xFF;
	}
	if(*encoder_right_button == BUTTON_PRESS_SHORT)
	{
		*encoder_right_button = BUTTON_PRESS_FREE;
		//radio_mute
	}
	if(*encoder_right_button == BUTTON_PRESS_LONG)
	{
		*encoder_right_button = BUTTON_PRESS_FREE;
		//radio_standby
	}
	return 0;
}
/*

	else {
				uint8_t temp[8];
				get_signal_qual(temp);
				//sprintf(rsq, "%d", temp[3] & 0x7F);
				//lcd_create_view(rsq, 8, 1, 0);
				sprintf(rsq, "  %ddBuV", temp[4]);
				lcd_create_view(rsq, 8 + (8 - strlen(rsq)), 1, 0);
				sprintf(rsq, "%ddB", temp[5]);
				lcd_create_view(rsq, (4 - strlen(rsq)), 2, 0);
				sprintf(rsq, "%d%c", temp[6], 0x25);
				lcd_create_view(rsq, 5 + (4 - strlen(rsq)), 2, 0);
				sprintf(rsq, "%dkHz", (int8_t)temp[7]);
				lcd_create_view(rsq, 11 + (5 - strlen(rsq)), 2, 0);
			}
*/
uint8_t radio_display_handler(void)
{
	char tmp_string[8];
	switch(radio.stats.mode) {
	case RADIO_RDS_VIEW:
		if(timer_count[2] == RADIO_TEXT_SCROLL) {
			if(radio.stats.scroll_text < 16) {
				lcd_create_view(radio.station_text, 15 - radio.stats.scroll_text, 2, radio.stats.scroll_text, 0);
			}
			else {
				lcd_create_view(radio.station_text - 15 + radio.stats.scroll_text, 0, 2, 16, 0);
			}
			if(++radio.stats.scroll_text >= 64) {
				radio.stats.scroll_text = 0;
			}
		}
		if(radio.status.rds_station_name == VALID) {
			lcd_create_view(radio.station_name, 0, 0, 0, 0);
		}
		else {
			radio_freq_to_string(tmp_string, radio.station_freq);
			lcd_create_view(tmp_string, 0, 0, 0, 0);
			lcd_create_view("MHz"); //TODO add this to a String table
		}

		date_to_str(tmp_string);
		lcd_create_view(tmp_string,  8, 0, 0, 0);
		break;
	case RADIO_RSQ_VIEW:
		radio_value_to_string(tmp_string, radio.rsq.rssi, 3);
		lcd_create_view(tmp_string, 8, 1, 0, 0);
		lcd_create_view("dBuV", 11, 11, 0, 0);
		radio_value_to_string(tmp_string, radio.rsq.snr, 3);
		lcd_create_view(tmp_string, 0, 2, 0, 0);
		lcd_create_view("dB", 3, 11, 0, 0);
		radio_value_to_string(tmp_string, radio.rsq.multi, 3);
		lcd_create_view(tmp_string, 5, 2, 0, 0);
		lcd_create_view("%", 11, 11, 0, 0);
		radio_value_to_string(tmp_string, radio.rsq.freq_off, 3);
		lcd_create_view(tmp_string, 8, 2, 0, 0);
		lcd_create_view("kHz", 11, 2, 0, 0);
		break;
	}

	time_to_str(tmp_string);
	lcd_create_view("\6", 9, 0, 0, 0); //TODO \6 add this to lcd symbols
	lcd_create_view(tmp_string,  10, 0, 0, 0);

	//TODO new radio handler which controll all for radio time and interupt based.
	return 0;
}

uint8_t radio_freq_to_string(char *str, uint16_t freq) {
	uint8_t count = 0;
	freq /= 10;
	str[0] = ' ';
	str[5] = 0;
	str += 4;
	while (freq > 0) {
		*str = freq % 10 + '0';
		str--;
		freq /= 10;
		if (count++ == 0) {
			*str = '.';
			str--;
		}
	}
	return 0;
}

uint8_t radio_value_to_string(char *str, uint16_t value, uint8_t size) {
	str[size] = 0;
	str += size - 1;
	while (size > 0) {
		if (value != 0) {
			*str = value % 10 + '0';
			value /= 10;
		}
		else {
			*str = ' ';
		}
		str--;
		size--;

	}
	return 0;
}


uint8_t radio_tune_freq(uint16_t freq)
{
	si4735_fm_tune_freq(freq);
	return 0;
}

uint8_t radio_rds_decoder()
{
	//TODO new rds decoder
	return 0;
}

uint16_t radio_seeking(uint8_t up_down)
{
	uint8_t resp[8];
	ext_interrupt_enable(SI_INT_INT);
	si4735_fm_seek_start(up_down);
	si4735_fm_tune_status(0, 1, resp);
	ext_interrupt_disable(SI_INT_INT);

	return (resp[2] << 8) + resp[3];
}

uint8_t radio_store_station(uint16_t freq, int8_t *name, uint8_t pos)
{

	uint8_t t_freq[2];
	t_freq[0] = (freq & 0x00FF);
	t_freq[1] = ((freq & 0xFF00) >> 8);
	store_data_to_flash(name, 8, pos * 8);
	store_data_to_flash(t_freq, 2, pos * 2 + 128);
	return 0;
}

uint16_t radio_read_station(int8_t *name, uint8_t pos)
{
	uint8_t t_freq[2];
	read_flash(name, 8, pos * 8);
	read_flash(t_freq, 2, 128 + pos * 2);
	return t_freq[0] + (t_freq[1] << 8);
}

uint8_t radio_auto_search()
{
	uint8_t resp[8];
	uint8_t station = 0;
	uint8_t freq = 0;
	si4735_fm_tune_freq(RADIO_BOT_FREQ);
	do {
		//TODO seek up to next channel
		freq = (radio_seeking(1)/10 - 875 +1);
		//(875 + (char)(act_freq-1))*10
		//Radio_States |= (1<<15);
		_delay_ms(10);
		//TODO wait for 10 ms for a vaild rds signal else store without
		//get_rds_data(&Radio_States, Station_Name, Radion_Text);
		//si4735_get_rds_data();
		//radio_store_station(freq, Station_Name, station);

		station++;

		// TODO Description for rds implementation, rds data will always be taken over handler if he get´s valids rds blocks.
		/* The rds_function gives back several stats like rds station name valid, rds programm text valid. Also it will handle
		 * after a radio station name got valid it will not be readed out again. To get a vaild radio station name there should
		 * be two or three time the same name from rds, after this i could be said that the radio station name is valid.
		 * Syncronization of the Clock should be all ways checked for valid this could be happen by checking time difference
		 * from one time to the other. The time difference should be counted after first time was gotten. The gap between the
		 * first and second valid time should be the difference, which was counted during this. For a valid rds text there should
		 * be two times the same. After that the function should be polling for a new radio text indicator.
		 */

		//TODO implement a better way to get rds data with a valid condition

	} while(0);//TODO do it untill top seek frequency is reached or station storage is full.
	//TODO Display list of founded Stations with a return mark on bottom.
	//TODO Go to first valid Station.
	//TODO mute SI4735 => go to lowest possible frequency => configure valid signal => start seeking up => wait valid channel interrupt => wait for valid rds information only station name => overwrite rom with new channel data => seek up agian
	//																							|																																			|
	//																							--------------------------------------------------------------------------------------------------------------------------------------------- => until highest frequency is reached
	return 0;
}
