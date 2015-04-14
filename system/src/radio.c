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
	uint8_t *init_ptr = (uint8_t *)RADIO_INIT_STORE_ADR;
	basic_clock_init();

	timer_init();
	i2c_init (400,10);

	if(*init_ptr != 0xAA) {
		radio_factory_state();
	}

	radio_load_settings();
	pca9530_init(&pca9530_config);
	pca9530_set_pwm(PWM_0, 256-exp_table[(uint8_t)(radio.settings.brightness * 2)]);
	radio_source_select(0);
	lcd_init(radio.settings.contrast);
	lcd_create_view(startup_line_1, 2, 0, 0, 0);
	lcd_create_view(startup_line_2, 2, 1, 0, 0);
	lcd_create_view(startup_line_3, 2, 2, 0, 1);
	tpa2016d2_init(radio.settings.equalizer,AMPLIFIER_GAIN);
	SI4735_INIT();
	radio_set_volume(radio.settings.volume);
	if(radio.settings.frequency < RADIO_BOT_FREQ) {
		radio.settings.frequency = radio_seeking(1);
	}
	else if(radio.settings.frequency > RADIO_TOP_FREQ) {
		radio.settings.frequency = radio_seeking(0);
	}
	else {
		radio_tune_freq(radio.settings.frequency);
	}
	Encoder_1_init();
	Encoder_2_init();
	return 0;
}

uint8_t radio_brightness(uint8_t *encoder_right_button, int8_t *encoder_right_count)
{
	if(*encoder_right_count != 0) {
		if(*encoder_right_count > 0 && radio.settings.brightness < RADIO_BRIGHTNESS_MAX) {
			radio.settings.brightness += RADIO_BRIGHTNESS_STEP;
		}
		else if(*encoder_right_count < 0 && radio.settings.brightness > RADIO_BRIGHTNESS_MIN){
			radio.settings.brightness -= RADIO_BRIGHTNESS_STEP;
		}
		pca9530_set_pwm(PWM_0, 256-exp_table[(uint8_t)(radio.settings.brightness * 2)]);
		*encoder_right_count = 0;
	}
	if(*encoder_right_button == BUTTON_PRESS_SHORT) {
		*encoder_right_button = BUTTON_FREE;
		radio_store_settings(0, 0);
		return SHORT_UP_TO_CHILD;
	}
	else {
		menu_scroll_settings(radio.settings.brightness * 2);
	}
	return STAY_ON_MENU_POINT;
}

uint8_t radio_contrast(uint8_t *encoder_right_button, int8_t *encoder_right_count)
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
	if(*encoder_right_button == BUTTON_PRESS_SHORT) {
		*encoder_right_button = BUTTON_FREE;
		radio_store_settings(0, 0);
		return SHORT_UP_TO_CHILD;
	}
	else {
		menu_scroll_settings(radio.settings.contrast * 10);
	}
	return STAY_ON_MENU_POINT;
}

uint8_t radio_volume(uint8_t *encoder_right_button, int8_t *encoder_right_count, uint8_t setting)
{
	static int8_t tmp_volume = 0;
	if(tmp_volume == 0 && setting != 0) {
		tmp_volume = radio.settings.volume;
		//TODO load volumes from store place
	}
	if(*encoder_right_count != 0) {
		if(*encoder_right_count > 0 && radio.settings.volume < RADIO_VOLUME_MAX) {
			radio.settings.volume += RADIO_VOLUME_STEP;
		}
		else if(*encoder_right_count < 0 && radio.settings.volume > RADIO_VOLUME_MIN) {
			radio.settings.volume -= RADIO_VOLUME_STEP;
		}
		radio_set_volume(radio.settings.volume);
		*encoder_right_count = 0;
	}
	if(*encoder_right_button == BUTTON_PRESS_SHORT) {
		*encoder_right_button = BUTTON_FREE;
		if(tmp_volume != 0) {
			if(setting == MENU_VOL_START_ENTRY) {
				radio_store_settings(0, 1);
			}
			else if(setting == MENU_VOL_TA_ENTRY) {
				radio.settings.volume_ta = radio.settings.volume;
				radio_store_settings(0, 0);
			}
			radio.settings.volume = tmp_volume;
			tmp_volume = 0;
			radio_set_volume(radio.settings.volume);
		}
		return SHORT_UP_TO_CHILD;
	}
	else {
		menu_scroll_settings(radio.settings.volume);
	}
	return STAY_ON_MENU_POINT;
}

uint8_t radio_source_select(uint8_t entry_num) {
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

uint8_t radio_equalizer(uint8_t entry_num)
{
	switch(entry_num) {
	case AUDIO_ROCK_ENTRY:
		radio.settings.equalizer = ROCK;
	case AUDIO_POP_ENTRY:
		radio.settings.equalizer = POP;
	case AUDIO_HIPHOP_ENTRY:
		radio.settings.equalizer = RAP_HIP_HOP;
	case AUDIO_NEWS_ENTRY:
		radio.settings.equalizer = NEWS_VOICE;
	case AUDIO_CLASSIC_ENTRY:
		radio.settings.equalizer = CLASSIC;;
	case AUDIO_JAZZ_ENTRY:
		radio.settings.equalizer = JAZZ;
	}
	tpa2016d2_equalizer_mode(radio.settings.equalizer, AMPLIFIER_GAIN);
	radio_store_settings(0, 0);
	return SHORT_UP_TO_PARENT;
}

uint8_t radio_settings(uint8_t *encoder_right_button, int8_t *encoder_right_count, uint8_t entry_num)
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
	case MENU_VOL_START_ENTRY:
	case MENU_VOL_TA_ENTRY:
		return radio_volume(encoder_right_button, encoder_right_count, entry_num);
	case SEEKING_UP:
		radio.settings.frequency = radio_seeking(1);
		return SHORT_UP_TO_PARENT;
	case SEEKING_DOWN:
		radio.settings.frequency = radio_seeking(0);
		return SHORT_UP_TO_PARENT;
	case STORE_STATION:
	case STATION_VIEW:
		return station_list_handler(encoder_right_button, encoder_right_count, entry_num);
	case AUTO_SEARCH:
		radio_auto_search();
		return SHORT_UP_TO_PARENT;
	case MENU_FREQ_SEEK_DOWN_ENTRY:
	case MENU_FREQ_SEEK_UP_ENTRY:
	case MENU_FREQ_CHOOSE_ENTRY:
		return radio_frequency(encoder_right_button, encoder_right_count, entry_num);
	case MENU_TA_TP_ON:
		radio.settings.ta_tp = 1;
		radio_store_settings(0, 0);
		*encoder_right_button = BUTTON_FREE;
		*encoder_right_count = 0;
		return SHORT_UP_TO_PARENT;
	case MENU_TA_TP_OFF:
		radio.settings.ta_tp = 0;
		radio_store_settings(0, 0);
		*encoder_right_button = BUTTON_FREE;
		*encoder_right_count = 0;
		return SHORT_UP_TO_PARENT;
	default:
		*encoder_right_button = BUTTON_FREE;
		*encoder_right_count = 0;
		return SHORT_UP_TO_CHILD;
	}
}


uint8_t radio_frequency(uint8_t *encoder_right_button, int8_t *encoder_right_count, uint8_t entry_num)
{
	uint16_t tmp_freq = 0;
	tmp_freq = radio.settings.frequency;
	if(entry_num == MENU_FREQ_SEEK_UP_ENTRY) {
		radio.settings.frequency = 0;
		radio_store_settings(1, 0);
		radio.settings.frequency = tmp_freq;
		return SHORT_UP_TO_PARENT;
	}
	else if(entry_num ==  MENU_FREQ_SEEK_DOWN_ENTRY) {
		radio.settings.frequency = 0xFFFF;
		radio_store_settings(1, 0);
		radio.settings.frequency = tmp_freq;
		return SHORT_UP_TO_PARENT;
	}
	else {
		uint8_t ret = station_list_handler(encoder_right_button, encoder_right_count, entry_num);
		radio.settings.frequency = tmp_freq;
		return ret;
	}
}

#define RADIO_STORE_VALUE 1

uint8_t radio_store_settings(uint8_t freq, uint8_t volume)
{
	uint16_t tmp_frequency = 0;
	uint8_t tmp_volume = 0;
	RADIO_SETTINGS tmp_settings;
	flash_read(&tmp_settings, sizeof(tmp_settings), RADIO_SETTINGS_STORE_ADR);
	if(freq != RADIO_STORE_VALUE) {
		tmp_frequency = radio.settings.frequency;
		radio.settings.frequency = tmp_settings.frequency;
	}
	if(volume != RADIO_STORE_VALUE) {
		tmp_volume = radio.settings.volume;
		radio.settings.volume = tmp_settings.volume;
	}

	flash_store(&(radio.settings), sizeof(radio.settings), RADIO_SETTINGS_STORE_ADR);

	if(freq != RADIO_STORE_VALUE) {
		radio.settings.frequency = tmp_frequency;
	}

	if(volume != RADIO_STORE_VALUE) {
		radio.settings.volume = tmp_volume;
	}
	return 0;
}

uint8_t radio_load_settings()
{
	flash_read(&radio.settings, sizeof(radio.settings), RADIO_SETTINGS_STORE_ADR);
	return 0;
}

uint8_t radio_set_volume(int8_t volume)
{
	uint8_t tmp_volume;
	if(volume > 100) {
		volume = 100;
	} else if(volume < 0) {
		volume = 0;
	}
	tmp_volume = ((int16_t)(volume) * SI4735_VOLUME_MAX) / 100;
	ext_interrupt_enable(SI_INT_INT);
	si4735_set_property(RX_VOLUME, tmp_volume);
	ext_interrupt_disable(SI_INT_INT);
	_delay_ms(10);
	return 0;
}

uint8_t radio_main(uint8_t *encoder_left_button, int8_t *encoder_left_count, uint8_t *encoder_right_button, int8_t *encoder_right_count)
{
	uint8_t tmp_value = 0;
	uint8_t blend_scroll = 0;

	if(*encoder_right_count != 0) { //move to radio_volume and
		if(*encoder_right_count < 0 && radio.settings.volume > RADIO_VOLUME_MIN) {
			radio.settings.volume -= RADIO_VOLUME_STEP;
		}
		else if(*encoder_right_count > 0 && radio.settings.volume < RADIO_VOLUME_MAX) {
			radio.settings.volume += RADIO_VOLUME_STEP;
		}
		radio_set_volume(radio.settings.volume);
		if(radio.status.audio_status == AUDIO_MUTE) {
			tpa2016d2_mute(0);
		}
		radio.status.audio_status = AUDIO_VOLUME;
		tmp_value = radio.settings.volume;
		blend_scroll = 1;
		*encoder_right_count = 0;
	}

	if(*encoder_left_count != 0) {
		if(*encoder_left_count < 0) {
			radio.settings.frequency -= 10;
		}
		else if(*encoder_left_count > 0) {
			radio.settings.frequency += 10;
		}
		radio.status.freq_valid = 0;
		if(radio.settings.frequency < RADIO_BOT_FREQ){
			radio.settings.frequency = RADIO_TOP_FREQ;
		}
		if(radio.settings.frequency > RADIO_TOP_FREQ){
			radio.settings.frequency = RADIO_BOT_FREQ;
		}
		radio_tune_freq(radio.settings.frequency);
		tmp_value = ((radio.settings.frequency - RADIO_BOT_FREQ) * 10) / ((RADIO_TOP_FREQ - RADIO_BOT_FREQ) / 10);
		blend_scroll = 1;
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
			tpa2016d2_mute(0);
			radio.status.audio_status = 0;
		} else {
			radio.status.audio_status = AUDIO_MUTE;
			tpa2016d2_mute(1);
		}
	}
	if(*encoder_left_button == BUTTON_PRESS_LONG)
	{
		*encoder_left_button = BUTTON_FREE;
		//radio_standby
	}

	if(radio.settings.ta_tp == 1 && radio.rds.ta == 1 && radio.rds.tp == 1 && radio.status.volume_ta == 0) {
		radio_set_volume(radio.settings.volume_ta);
		radio.status.volume_ta = 1;
	} else if(radio.rds.ta == 0 && radio.status.volume_ta == 1) {
		radio_set_volume(radio.settings.volume);
		radio.status.volume_ta = 0;
	}

	radio_display_handler(blend_scroll, tmp_value);
	return 0;
}

uint8_t radio_auto_brightness()
{
	static uint32_t brightness_value = 0;
	uint32_t tmp_value;
	uint8_t i = 0;
	opt3001_get_value(&tmp_value);
	if(brightness_value == 0) {
		for(i = 0; i < 64; i++) {
			brightness_value += tmp_value;
		}
	}
	else {
		brightness_value -= brightness_value >> 6;
		brightness_value += tmp_value;
	}
	//TODO calculate a brightness value for background brightness
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
	ext_interrupt_disable(SI_INT_INT);
	si4735_fm_tune_status(1, 1, resp);

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
	uint16_t try_rds = 0;
	uint8_t percentage = 0;
	char tmp_string[4];
	do {
		//TODO seek up to next channel
		radio.settings.frequency = radio_seeking(1);
		if(station == 0) {
			freq = radio.settings.frequency;
		}
		else {
			if(freq == radio.settings.frequency){
				break;
			}
		}
		radio.status.freq_valid = 0;

		if(radio.settings.frequency < freq) {
			percentage = ((freq - radio.settings.frequency) * 10) / ((RADIO_TOP_FREQ - RADIO_BOT_FREQ) / 10);
		}
		else {
			percentage = ((radio.settings.frequency - freq) * 10) / ((RADIO_TOP_FREQ - RADIO_BOT_FREQ) / 10);
		}

		lcd_create_view(auto_search_text, 0, 0, 0, 0);
		lcd_create_view(progress_text, 0, 1, 0, 0);
		string_int_to_array(tmp_string, percentage, 3, 10);
		lcd_create_view(tmp_string, 12, 1, 0, 0);
		lcd_create_view("%", 15, 1, 0, 1);

		do {
			_delay_ms(2);
			rds_update(&radio);
		}while(radio.status.name_valid != VALID && try_rds++ < 3000);
		try_rds = 0;
		radio_store_station(&(radio.settings.frequency), radio.rds.name, station);
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

uint8_t radio_factory_state()
{
	uint8_t i = 0;
	radio.settings.frequency = 0;
	radio.settings.display_view = RADIO_RDS_VIEW;
	radio.settings.volume = 20;
	radio.settings.equalizer = POP;
	radio.settings.source = SOURCE_FM;
	radio.settings.brightness = 35;
	radio.settings.contrast = 6;

	for(i = 0; i < 14; i++) {
		flash_store((char *)empty_text, RADIO_STATION_NAME_STORE_SIZE, RADIO_STATION_NAME_STORE_ADR + RADIO_STATION_NAME_STORE_SIZE * i);
		flash_store(&i, RADIO_STATION_FREQ_STORE_SIZE, RADIO_STATION_FREQ_STORE_ADR + RADIO_STATION_FREQ_STORE_SIZE * i);
	}

	radio_store_settings(1, 1);

	i = 0xAA;

	flash_store(&i, 1, RADIO_INIT_STORE_ADR);

	return 0;
}

uint8_t radio_stand_by()
{
	//TODO Turn off amplifier
	//TODO Turn off si4735
	//TODO store actuall freqency
	//TODO store actuall volume
	//TODO Turn off display
	//TODO enable interrupt for left button
	//TODO reconfig one timer with ACLK to cause an interrupt all minute
	//TODO go to lpm mode where ACLK is active
	//TODO wait until button is pressed
	//TODO reconfig timer to old state
	//TODO enable amplifier with stored values
	//TODO enable si4735 with stored values
	//TODO turn on display needed to be init
	//TODO go back to normal operations
	return 0;
}
