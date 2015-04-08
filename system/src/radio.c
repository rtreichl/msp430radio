/*
 * radio_new.c
 *
 *  Created on: 31.01.2015
 *      Author: Richard
 */

#include <system/radio.h>

RADIO radio;

void rds_update(RADIO *radio);

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
	radio.status.audio_status = 0;
	radio.status.display_mode = RADIO_RDS_VIEW;
	radio.volume = 30;
	radio.status.source_select = SOURCE_FM;
	radio.brightness = 80;
	radio.contrast = 40;
	//TODOload values from flash
	pca9530_init(&pca9530_config);
	pca9530_set_pwm(PWM_0, 256-exp_table[(uint8_t)(radio.brightness)]);
	radio_source_select(0);
	lcd_init(radio.contrast / RADIO_CONTRAST_STEP);
	lcd_create_view(Start_up_1, Shift_left_1, 0, 0, 0);
	lcd_create_view(Start_up_2, Shift_left_2, 1, 0, 0);
	lcd_create_view(Start_up_3, Shift_left_3, 2, 0, 1);
	tpa2016d2_init(POP,AMPLIFIER_GAIN);
	SI4735_INIT();
	radio_set_volume(&(radio.volume));
	Encoder_1_init();
	Encoder_2_init();
	return 0;
}

uint8_t radio_brightness(uint8_t *encoder_right_button, int8_t *encoder_right_count)
{
	if(*encoder_right_count != 0) {
		if(*encoder_right_count > 0 && radio.brightness < RADIO_BRIGHTNESS_MAX) {
			radio.brightness += RADIO_BRIGHTNESS_STEP;
		}
		else if(*encoder_right_count < 0 && radio.brightness > RADIO_BRIGHTNESS_MIN){
			radio.brightness -= RADIO_BRIGHTNESS_STEP;
		}
		pca9530_set_pwm(PWM_0, 256-exp_table[(uint8_t)(radio.brightness)]);
		*encoder_right_count = 0;
	}
	if(*encoder_right_button == BUTTON_PRESS_SHORT) {
		*encoder_right_button = BUTTON_FREE;
		flash_store(&(radio.brightness), RADIO_BRIGHTNESS_STORE_SIZE, RADIO_BRIGHTNESS_STORE_ADR);
		return SHORT_UP_TO_CHILD;
	}
	else {
		menu_scroll_settings(radio.brightness);
	}
	return STAY_ON_MENU_POINT;
}

uint8_t radio_contrast(uint8_t *encoder_right_button, int8_t *encoder_right_count)
{
	if(*encoder_right_count != 0) {
		if(*encoder_right_count > 0 && radio.contrast < RADIO_CONTRAST_MAX) {
			radio.contrast += RADIO_CONTRAST_STEP;
		}
		else if(*encoder_right_count < 0 && radio.contrast > RADIO_CONTRAST_MIN) {
			radio.contrast -= RADIO_CONTRAST_STEP;
		}
		lcd_contrast(radio.contrast / RADIO_CONTRAST_STEP);
		*encoder_right_count = 0;
	}
	if(*encoder_right_button == BUTTON_PRESS_SHORT) {
		*encoder_right_button = BUTTON_FREE;
		flash_store(&(radio.contrast), RADIO_CONTRAST_STORE_SIZE, RADIO_CONTRAST_STORE_ADR);
		return SHORT_UP_TO_CHILD;
	}
	else {
		menu_scroll_settings(radio.contrast);
	}
	return STAY_ON_MENU_POINT;
}

uint8_t radio_volume(uint8_t *encoder_right_button, int8_t *encoder_right_count, uint8_t setting)
{
	static int8_t tmp_volume = 0;
	if(tmp_volume == 0 && setting == 1) {
		tmp_volume = radio.volume;
	}
	if(*encoder_right_count != 0) {
		if(*encoder_right_count > 0 && radio.volume < RADIO_VOLUME_MAX) {
			radio.volume += RADIO_VOLUME_STEP;
		}
		else if(*encoder_right_count < 0 && radio.volume> RADIO_VOLUME_MIN) {
			radio.volume -= RADIO_VOLUME_STEP;
		}
		radio_set_volume(&(radio.volume));
		*encoder_right_count = 0;
	}
	if(*encoder_right_button == BUTTON_PRESS_SHORT) {
		*encoder_right_button = BUTTON_FREE;
		if(tmp_volume != 0) {
			radio.volume = tmp_volume;
			flash_store(&(radio.volume), RADIO_VOLUME_STORE_SIZE, RADIO_VOLUME_STORE_ADR);
			tmp_volume = 0;
			radio_set_volume(&(radio.volume));
		}
		return SHORT_UP_TO_CHILD;
	}
	else {
		menu_scroll_settings(radio.volume);
	}
	return STAY_ON_MENU_POINT;
}

uint8_t radio_source_select(uint8_t entry_num) {
	AUDIO_SW_LINE_DIR |= AUDIO_SW_LINE_PIN;
	AUDIO_SW_GND_DIR |= AUDIO_SW_GND_PIN;
	switch(entry_num) {
	default:
	case SOURCE_FM_ENTRY:
		radio.status.source_select = SOURCE_FM;
		AUDIO_SW_LINE_OUT &= ~AUDIO_SW_LINE_PIN;
		AUDIO_SW_GND_OUT &= ~AUDIO_SW_GND_PIN;
		//TODO Implement switch si4735 to FM only when AM mode is implemented else just switch audio switch
		break;
	case SOURCE_AM_ENTRY:
		radio.status.source_select = SOURCE_AM;
		AUDIO_SW_LINE_OUT &= ~AUDIO_SW_LINE_PIN;
		AUDIO_SW_GND_OUT &= ~AUDIO_SW_GND_PIN;
		//TODO Implement switch si4735 to AM only when AM mode is implemented else just do nothing else
		break;
	case SOURCE_LINEIN_ENTRY:
		radio.status.source_select = SOURCE_LINEIN;
		AUDIO_SW_LINE_OUT |= AUDIO_SW_LINE_PIN;
		AUDIO_SW_GND_OUT |= AUDIO_SW_GND_PIN;
		//TODO set si4735 in powerdown modus only if ta/tp mode is off else poll flag and swith to si4735 for duration ta flag is set
	}
	//flash_store(&(radio.status.source_select), RADIO_SOURCE_STORE_SIZE, RADIO_SOURCE_STORE_ADR);
	return SHORT_UP_TO_PARENT;
}

uint8_t radio_equalizer(uint8_t entry_num)
{
	switch(entry_num) {
	case AUDIO_ROCK_ENTRY:
		radio.status.equalizer_mode = ROCK;
	case AUDIO_POP_ENTRY:
		radio.status.equalizer_mode = POP;
	case AUDIO_HIPHOP_ENTRY:
		radio.status.equalizer_mode = RAP_HIP_HOP;
	case AUDIO_NEWS_ENTRY:
		radio.status.equalizer_mode = NEWS_VOICE;
	case AUDIO_CLASSIC_ENTRY:
		radio.status.equalizer_mode = CLASSIC;;
	case AUDIO_JAZZ_ENTRY:
		radio.status.equalizer_mode = JAZZ;
	}
	tpa2016d2_equalizer_mode(radio.status.equalizer_mode, AMPLIFIER_GAIN);
	//flash_store(&(radio.status.equalizer_mode), RADIO_EQUALIZER_STORE_SIZE, RADIO_EQUALIZER_STORE_ADR);
	return SHORT_UP_TO_PARENT;
}

uint8_t radio_settings(uint8_t *encoder_right_button, int8_t *encoder_right_count, uint8_t entry_num)
{
	switch(entry_num) {
	case RADIO_RDS_VIEW_ENTRY:
		radio.status.display_mode = RADIO_RDS_VIEW;
		//flash_store(&(radio.status.display_mode), RADIO_VIEW_STORE_SIZE, RADIO_VIEW_STORE_ADR);
		return SHORT_UP_TO_PARENT;
	case RADIO_RSQ_VIEW_ENTRY:
		radio.status.display_mode = RADIO_RSQ_VIEW;
		//flash_store(&(radio.status.display_mode), RADIO_VIEW_STORE_SIZE, RADIO_VIEW_STORE_ADR);
		return SHORT_UP_TO_PARENT;
	case RADIO_PIPTY_VIEW_ENTRY:
		radio.status.display_mode = RADIO_PIPTY_VIEW;
		//flash_store(&(radio.status.display_mode), RADIO_VIEW_STORE_SIZE, RADIO_VIEW_STORE_ADR);
		return SHORT_UP_TO_PARENT;
	case AUDIO_ROCK_ENTRY:
	case AUDIO_POP_ENTRY:
	case AUDIO_HIPHOP_ENTRY:
	case AUDIO_NEWS_ENTRY:
	case AUDIO_CLASSIC_ENTRY:
	case AUDIO_JAZZ_ENTRY:
		*encoder_right_button = BUTTON_FREE;
		*encoder_right_count = 0;
		return radio_equalizer(entry_num);
	case SOURCE_AM_ENTRY:
	case SOURCE_FM_ENTRY:
	case SOURCE_LINEIN_ENTRY:
		*encoder_right_button = BUTTON_FREE;
		*encoder_right_count = 0;
		return radio_source_select(entry_num);
	case MENU_BRIG_ENTRY:
		return radio_brightness(encoder_right_button, encoder_right_count);
	case MENU_CONT_ENTRY:
		return radio_contrast(encoder_right_button, encoder_right_count);
	case MENU_VOL_ENTRY:
		return radio_volume(encoder_right_button, encoder_right_count, 1);
	case SEEKING_UP:
		radio.station_freq = radio_seeking(1);
		return SHORT_UP_TO_PARENT;
	case SEEKING_DOWN:
		radio.station_freq = radio_seeking(0);
		return SHORT_UP_TO_PARENT;
	case STORE_STATION:
	case STATION_VIEW:
		return station_list_handler(encoder_right_button, encoder_right_count, entry_num);
	case AUTO_SEARCH:
		radio_auto_search();
		return SHORT_UP_TO_PARENT;
	default:
		*encoder_right_button = BUTTON_FREE;
		*encoder_right_count = 0;
		return SHORT_UP_TO_CHILD;
	}
}

uint8_t radio_set_volume(int8_t *volume)
{
	uint8_t tmp_volume;
	if(*volume > 100) {
		*volume = 100;
	} else if(*volume < 0) {
		*volume = 0;
	}
	tmp_volume = ((int16_t)(*volume) * SI4735_VOLUME_MAX) / 100;
	ext_interrupt_enable(SI_INT_INT);
	si4735_set_property(RX_VOLUME, tmp_volume);
	ext_interrupt_disable(SI_INT_INT);
	_delay_ms(10);
	return 0;
}

uint8_t radio_main(uint8_t *encoder_left_button, int8_t *encoder_left_count, uint8_t *encoder_right_button, int8_t *encoder_right_count)
{
	uint8_t tmp_value = 0;

	if(*encoder_right_count != 0) { //move to radio_volume and
		if(*encoder_right_count < 0 && radio.volume > 0) {
			radio.volume--;
		}
		else if(*encoder_right_count > 0 && radio.volume < 100) {
			radio.volume++;
		}
		radio_set_volume(&(radio.volume));
		radio.status.audio_status = AUDIO_VOLUME;
		tmp_value = radio.volume;
		*encoder_right_count = 0;
	}

	if(*encoder_left_count != 0) {
		if(*encoder_left_count < 0 && radio.station_freq > RADIO_BOT_FREQ) {
			radio.station_freq -= 10;
		}
		else if(*encoder_left_count > 0 && radio.station_freq < RADIO_TOP_FREQ) {
			radio.station_freq += 10;
		}
		radio.status.freq_valid = 0;
		radio_tune_freq(radio.station_freq);
		tmp_value = ((radio.station_freq - RADIO_BOT_FREQ) * 10) / ((RADIO_TOP_FREQ - RADIO_BOT_FREQ) / 10);
		*encoder_left_count = 0;
	}
	if(*encoder_right_button == BUTTON_PRESS_SHORT)
	{
		*encoder_right_button = BUTTON_FREE;
		return SHORT_INTO_MENU;
	}
	if(*encoder_right_button == BUTTON_PRESS_LONG)
	{
		*encoder_right_button = BUTTON_FREE;
		return LONG_INTO_MENU;
	}
	if(*encoder_left_button == BUTTON_PRESS_SHORT)
	{
		*encoder_left_button = BUTTON_FREE;
		if(radio.status.audio_status == AUDIO_MUTE) {
			radio.status.audio_status = 0;
		} else {
			radio.status.audio_status = AUDIO_MUTE;
		}
		//radio_mute
	}
	if(*encoder_left_button == BUTTON_PRESS_LONG)
	{
		*encoder_left_button = BUTTON_FREE;
		//radio_standby
	}
	radio_display_handler(tmp_value);
	return 0;
}

uint8_t radio_tune_freq(uint16_t freq)
{
	ext_interrupt_enable(SI_INT_INT);
	si4735_fm_tune_freq(freq);
	ext_interrupt_disable(SI_INT_INT);
	return 0;
}

uint16_t radio_seeking(uint8_t up_down)
{
	uint8_t resp[8];
	ext_interrupt_enable(SI_INT_INT);
	si4735_fm_seek_start(up_down);
	si4735_fm_tune_status(0, 1, resp);
	ext_interrupt_disable(SI_INT_INT);

	radio.status.freq_valid = 0;

	return (resp[2] << 8) + resp[3];
}

uint8_t radio_store_station(uint16_t *freq, char *name, uint8_t pos)
{
	char tmp_string[8];
	if(radio.status.name_valid != VALID) {
		string_fixedpoint_to_array(tmp_string, *freq);
		tmp_string[5] = 'M';
		tmp_string[6] = 'H';
		tmp_string[7] = 'z';
		flash_store(tmp_string, RADIO_STATION_NAME_STORE_SIZE, RADIO_STATION_NAME_STORE_ADR + RADIO_STATION_NAME_STORE_SIZE * pos);
	}
	else {
		flash_store(name, RADIO_STATION_NAME_STORE_SIZE, RADIO_STATION_NAME_STORE_ADR + RADIO_STATION_NAME_STORE_SIZE * pos);
	}
	flash_store(freq, RADIO_STATION_FREQ_STORE_SIZE, RADIO_STATION_FREQ_STORE_ADR + RADIO_STATION_FREQ_STORE_SIZE * pos);
	return 0;
}

uint16_t radio_read_station(int8_t *name, uint8_t pos)
{
	uint8_t t_freq[2];
	flash_read(name, 8, pos * 8);
	flash_read(t_freq, 2, 128 + pos * 2);
	return t_freq[0] + (t_freq[1] << 8);
}

uint8_t radio_auto_search()
{
	uint8_t station = 0;
	uint16_t freq = 0;
	do {
		//TODO seek up to next channel
		radio.station_freq = radio_seeking(1);
		if(station == 0) {
			freq = radio.station_freq;
		}
		else {
			if(freq == radio.station_freq){
				break;
			}
		}
		radio.status.freq_valid = 0;
		do {
			_delay_ms(5);
			rds_update(&radio);
		}while(radio.status.name_valid != VALID);

		radio_store_station(&(radio.station_freq), radio.rds.name, station);

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

	} while(1);//TODO do it untill top seek frequency is reached or station storage is full.
	//TODO Display list of founded Stations with a return mark on bottom.
	//TODO Go to first valid Station.
	//TODO mute SI4735 => go to lowest possible frequency => configure valid signal => start seeking up => wait valid channel interrupt => wait for valid rds information only station name => overwrite rom with new channel data => seek up agian
	//																							|																																			|
	//																							--------------------------------------------------------------------------------------------------------------------------------------------- => until highest frequency is reached
	return 0;
}
