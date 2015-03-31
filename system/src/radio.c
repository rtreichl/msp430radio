/*
 * radio_new.c
 *
 *  Created on: 31.01.2015
 *      Author: Richard
 */

#include <system/radio.h>

RADIO radio;

#define AUDIO_MUTE 1

const PCA9530 pca9530_config = {
	76,
	0,
	1,
	127,
	PWM0_RATE,
	PWM1_RATE
};

uint8_t radio_init()
{
	basic_clock_init();
	timer_init();
	i2c_init (400,10);
	radio.station_freq = 10770;
	radio.status.audio_mute = 0;
	radio.status.display_mode = 2;
	radio.volume = 30;
	radio.brightness = 80;
	radio.contrast = 4;
	//TODOload values from flash
	pca9530_init(&pca9530_config);
	radio_brightness(radio.brightness);
	lcd_init(radio.contrast);
	lcd_create_view(Start_up_1, Shift_left_1, 0, 0, 0);
	lcd_create_view(Start_up_2, Shift_left_2, 1, 0, 0);
	lcd_create_view(Start_up_3, Shift_left_3, 2, 0, 1);
	Amplifier_init(POP,AMPLIFIER_GAIN);
	SI4735_INIT();
	radio_volume(&(radio.volume));
	Encoder_1_init();
	Encoder_2_init();
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

uint8_t radio_volume(int8_t *volume)
{
	uint8_t tmp_volume;
	if(*volume > 100) {
		*volume = 100;
	} else if(*volume < 0) {
		*volume = 0;
	}
	tmp_volume = ((int16_t)(*volume) * SI4735_VOLUME_MAX) / 100;
	si4735_set_property(RX_VOLUME, tmp_volume);
	_delay_ms(10);
	return 0;
}

uint8_t radio_main(uint8_t *encoder_left_button, int8_t *encoder_left_count)
{
	uint8_t encoder_right_button;
	int8_t encoder_right_count;

	if(*encoder_left_count != 0) {
		if(*encoder_left_count < 0 && radio.volume > 0) {
			radio.volume--;
			radio_volume(&(radio.volume));
			*encoder_left_count = 0;
		}
		else if(*encoder_left_count > 0 && radio.volume < 100) {
			radio.volume++;
			radio_volume(&(radio.volume));
			*encoder_left_count = 0;
		}
	}

	if(encoder_right_count != 0) {
		if(encoder_right_count < 0 && radio.station_freq > 0) {
			radio.station_freq--;
			encoder_right_count = 0;
		}
		else if(encoder_right_count > 0 && radio.station_freq < 100) {
			radio.station_freq++;
			encoder_right_count = 0;
		}
	}
	if(*encoder_left_button == BUTTON_PRESS_SHORT)
	{
		*encoder_left_button = BUTTON_FREE;
		return 0xFF;
	}
	if(*encoder_left_button == BUTTON_PRESS_LONG)
	{
		*encoder_left_button = BUTTON_FREE;
		return 0xFF;
	}
	if(encoder_right_button == BUTTON_PRESS_SHORT)
	{
		encoder_right_button = BUTTON_FREE;
		//radio_mute
	}
	if(encoder_right_button == BUTTON_PRESS_LONG)
	{
		encoder_right_button = BUTTON_FREE;
		//radio_standby
	}
	radio_display_handler();
	return 0;
}

#define RADIO_TEXT_SCROLL 500

uint8_t radio_display_handler(void)
{
	char tmp_string[9];
	switch(radio.status.display_mode) {
	case RADIO_RDS_VIEW:
		if(timer_count[2] >= RADIO_TEXT_SCROLL) {
			timer_count[2] -= RADIO_TEXT_SCROLL;
			if(radio.status.scroll_text < 15) {
				//lcd_create_view(radio.rds.text, 15 - radio.status.scroll_text, 2, radio.status.scroll_text, 0);
			}
			else {
				//lcd_create_view(radio.rds.text - 15 + radio.status.scroll_text, 0, 2, 16, 0);
			}
			if(++radio.status.scroll_text >= 64) {
				radio.status.scroll_text = 0;
			}
		}
		date_to_str(tmp_string);
		lcd_create_view(tmp_string,  8, 1, 0, 0);
		break;
	case RADIO_RSQ_VIEW:
		radio_value_to_string(tmp_string, radio.rsq.rssi, 3, 10);
		lcd_create_view(tmp_string, 9, 1, 0, 0);
		lcd_create_view("dBuV", 12, 1, 0, 0);
		radio_value_to_string(tmp_string, radio.rsq.snr, 3, 10);
		lcd_create_view(tmp_string, 0, 2, 0, 0);
		lcd_create_view("dB", 3, 2, 0, 0);
		radio_value_to_string(tmp_string, radio.rsq.multi, 3 ,10);
		lcd_create_view(tmp_string, 6, 2, 0, 0);
		lcd_create_view("%", 9, 2, 0, 0);
		radio_value_to_string(tmp_string, radio.rsq.freq_off, 3, 10);
		lcd_create_view(tmp_string, 10, 2, 0, 0);
		lcd_create_view("kHz", 13, 2, 0, 0);
		break;
	case RADIO_PIPTY_VIEW:
		radio_value_to_string(tmp_string, radio.rds.pi, 4, 16);
		lcd_create_view("PI:", 9, 1, 0, 0);
		lcd_create_view(tmp_string, 12, 1, 0, 0);
		lcd_create_view(pty_text[radio.rds.pty], 0, 2, 0, 0);
		break;
	}
	if(radio.status.name_valid == VALID) {
		lcd_create_view(radio.rds.name, 0, 0, 0, 0);
	}
	else {
		radio_freq_to_string(tmp_string, radio.station_freq);
		lcd_create_view(tmp_string, 0, 0, 0, 0);
		lcd_create_view("MHz", 5, 0, 0, 0); //TODO add this to a String table
	}
	time_to_str(tmp_string);
	lcd_create_view("\6", 10, 0, 0, 0); //TODO \6 add this to lcd symbols
	if(radio.status.audio_mute == AUDIO_MUTE) {
		lcd_create_view("\7÷", 0, 1, 0, 0);
	} else {
		lcd_create_view("\7û", 0, 1, 0, 0);	//Audio normal
	}
	lcd_create_view(tmp_string,  11, 0, 0, 1);

	//TODO new radio handler which controll all for radio time and interupt based.
	return 0;
}

uint8_t radio_freq_to_string(char *str, uint16_t freq)
{
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

uint8_t radio_value_to_string(char *str, int16_t value, uint8_t size, uint8_t base)
{
	uint8_t tmp_sign = 0;
	str[size] = 0;
	str += size - 1;
	if(value == 0) {
		*str = '0';
		str--;
	} else if (value < 0) {
		tmp_sign = 1;
		value = (uint16_t)(-1 * value);
	}

	while (size > 0) {
		if (value != 0) {
			*str = value % base + '0';
			if (*str > '9') {
				*str += 'A' - '9' - 1;
			}
			value /= base;
		}
		else {
			if(tmp_sign == 1) {
				*str = '-';
				tmp_sign = 0;
			} else {
				*str = ' ';
			}
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
