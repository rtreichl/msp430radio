/*
 * radio_new.c
 *
 *  Created on: 31.01.2015
 *      Author: Richard
 */

#include <radio/radio.h>

//----------------------------------------------------------------------------------------
//
/// \brief Definition of globals, structs and functions
//
//----------------------------------------------------------------------------------------

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

//----------------------------------------------------------------------------------------
//
/// \brief Initiales the radio
//
///	(1)Clock Init\n
///	(2)Timer Init\n
///	(3)Load stored radio settings\n
///	(4)Init PCA9530\n
///	(5)LCD Display Init\n
///	(6)Write start text\n
///	(7)TPA Init\n
///	(8)Si4735 Init\n
///	(9)Init both encoders
/// \param
//
/// \retval uint8_t
//
/// \remarks
//
//----------------------------------------------------------------------------------------

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
	radio_brightness(radio.settings.brightness);
	radio_settings_source(0, 0, 0, 0, 0);
	lcd_init(radio.settings.contrast);
	lcd_create_view(startup_line_1, 2, 0, 0, 0);
	lcd_create_view(startup_line_2, 2, 1, 0, 0);
	lcd_create_view(startup_line_3, 2, 2, 0, 1);
	tpa2016d2_init((enum TPA2016D2_EQUALIZER)radio.settings.equalizer, RADIO_AMPLIFIER_GAIN);
	SI4735_INIT();
	radio_volume(radio.settings.volume);
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

//----------------------------------------------------------------------------------------
//
/// \brief Store the current settings
//
/// \param	<freq>		[in]	Indicator to save the frequency
/// \param	<volume>	[in]	Indicator to save the volume
//
/// \retval uint8_t
//
/// \remarks
//
//----------------------------------------------------------------------------------------

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

//----------------------------------------------------------------------------------------
//
/// \brief Load stored settings
//
/// \param
//
/// \retval uint8_t
//
/// \remarks
//
//----------------------------------------------------------------------------------------

uint8_t radio_load_settings()
{
	flash_read(&radio.settings, sizeof(radio.settings), RADIO_SETTINGS_STORE_ADR);
	return 0;
}

uint8_t radio_brightness(uint8_t brightness)
{
	if(brightness == 0) {
		pca9530_config_leds(LED_ON, PWM1_RATE);
	}
	else {
		pca9530_config_leds(PWM0_RATE, PWM1_RATE);
		pca9530_set_pwm(PWM_0, 255-exp_table[brightness * 2 - 1]);
	}
	return 0;
}

//----------------------------------------------------------------------------------------
//
/// \brief Set the volume
//
/// \param	<volume>	[in]	Volume to be set
//
/// \retval uint8_t
//
/// \remarks
//
//----------------------------------------------------------------------------------------

uint8_t radio_volume(int8_t volume)
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

//----------------------------------------------------------------------------------------
//
/// \brief Radio main
//
///	Entry to the sub menus, set the frequency and actualize the display according to the
///	data to be displayed
//
/// \param	<encoder_left_button>	[in]	State of the left button
/// \param	<encoder_left_count>	[in]	Count of the left encoder
/// \param	<encoder_right_button>	[in]	State of the right button
/// \param	<encoder_right_count>	[in]	Count of the right encoder
/// \param	<entry_num>				[in]	Number of the entry
//
/// \retval
//
/// \remarks
//
//----------------------------------------------------------------------------------------

uint8_t radio_main(uint8_t *encoder_left_button, int8_t *encoder_left_count, uint8_t *encoder_right_button, int8_t *encoder_right_count, uint8_t entry_num)
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
		radio_volume(radio.settings.volume);
		if(radio.status.audio_status == AUDIO_MUTE) {
			tpa2016d2_muting(TPA2016D2_OUTPUT);
		}
		radio.status.audio_status = AUDIO_VOLUME;
		tmp_value = radio.settings.volume;
		blend_scroll = 1;
		*encoder_right_count = 0;
	}

	if(*encoder_left_count != 0) {
		if(*encoder_left_count < 0) {
			radio.settings.frequency -= RADIO_FREQENCY_STEP;
		}
		else if(*encoder_left_count > 0) {
			radio.settings.frequency += RADIO_FREQENCY_STEP;
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
	if(*encoder_right_button == BUTTON_SHORT)
	{
		*encoder_right_button = BUTTON_FREE;
		return SHORT_INTO_MENU;
	}
	if(*encoder_right_button == BUTTON_LONG)
	{
		*encoder_right_button = BUTTON_FREE;
		return LONG_INTO_MENU;
	}
	if(*encoder_left_button == BUTTON_SHORT)
	{
		*encoder_left_button = BUTTON_FREE;
		if(radio.status.audio_status == AUDIO_MUTE) {
			tpa2016d2_muting(TPA2016D2_OUTPUT);
			radio.status.audio_status = 0;
		} else {
			radio.status.audio_status = AUDIO_MUTE;
			tpa2016d2_muting(TPA2016D2_MUTE);
		}
	}
	if(*encoder_left_button == BUTTON_LONG)
	{
		*encoder_left_button = BUTTON_FREE;
		//radio_standby
	}

	if(radio.settings.ta_tp == 1 && radio.rds.ta == 1 && radio.rds.tp == 1 && radio.status.volume_ta == 0) {
		radio_volume(radio.settings.volume_ta);
		radio.status.volume_ta = 1;
	} else if(radio.rds.ta == 0 && radio.status.volume_ta == 1) {
		radio_volume(radio.settings.volume);
		radio.status.volume_ta = 0;
	}

	radio_display_handler(blend_scroll, tmp_value);
	return 0;
}

//----------------------------------------------------------------------------------------
//
/// \brief Set Auto brightness
//
/// \param
//
/// \retval uint8_t
//
/// \remarks
//
//----------------------------------------------------------------------------------------

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

//----------------------------------------------------------------------------------------
//
/// \brief Tune the frequency
//
/// \param	<freq>	[in]	Frequency to be tuned
//
/// \retval uint8_t
//
/// \remarks
//
//----------------------------------------------------------------------------------------

uint8_t radio_tune_freq(uint16_t freq)
{
	ext_interrupt_enable(SI_INT_INT);
	si4735_fm_tune_freq(freq);
	ext_interrupt_disable(SI_INT_INT);
	return 0;
}

//----------------------------------------------------------------------------------------
//
/// \brief Seeking all radio channels
//
/// \param	<up_down>	[in]	Indicator if to seek up or down
//
/// \retval uint16_t
//
/// \remarks
//
//----------------------------------------------------------------------------------------

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

//----------------------------------------------------------------------------------------
//
/// \brief Store one radio station
//
/// \param	<freq>	[in]	Frequency of the radion station
/// \param	<name>	[in]	Name of the radio station
/// \param	<pos>	[in]	Position in the store list
//
/// \retval uint8_t
//
/// \remarks
//
//----------------------------------------------------------------------------------------

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

//----------------------------------------------------------------------------------------
//
/// \brief Set one radio station to the current
//
/// \param	<name>	[in]	Name of the radio station
/// \param	<name>	[in]	Position of the radio station in the list
//
/// \retval uint16_t
//
/// \remarks
//
//----------------------------------------------------------------------------------------

uint16_t radio_read_station(int8_t *name, uint8_t pos)
{
	uint8_t t_freq[2];
	flash_read(name, 8, pos * 8);
	flash_read(t_freq, 2, 128 + pos * 2);
	return t_freq[0] + (t_freq[1] << 8);
}

//----------------------------------------------------------------------------------------
//
/// \brief Auto Search for radio stations
//
/// \param
//
/// \retval uint8_t
//
/// \remarks
//
//----------------------------------------------------------------------------------------

uint8_t radio_auto_search()
{
	uint8_t station = 0;
	uint16_t freq = 0;
	uint16_t try_rds = 0;
	uint8_t percentage = 0;
	char tmp_string[4];
	do {
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
			percentage = ((RADIO_TOP_FREQ - RADIO_BOT_FREQ - freq + radio.settings.frequency) * 10) / ((RADIO_TOP_FREQ - RADIO_BOT_FREQ) / 10);
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

//----------------------------------------------------------------------------------------
//
/// \brief Factory reset
//
///	(1)Set the Radio object
///	(2)Store the radio settings
/// (3)Store it to flash
//
/// \param
//
/// \retval uint8_t
//
/// \remarks
//
//----------------------------------------------------------------------------------------

uint8_t radio_factory_state()
{
	uint8_t i = 0;
	radio.settings.frequency = RADIO_FACORY_FREQENCY;
	radio.settings.display_view = RADIO_FACORY_VIEW;
	radio.settings.volume = RADIO_FACORY_VOLUME;
	radio.settings.ta_tp = RADIO_FACORY_TATP;
	radio.settings.volume_ta = RADIO_FACORY_VOLUME_TA;
	radio.settings.equalizer = RADIO_FACORY_EQUALIZER;
	radio.settings.source = RADIO_FACORY_SOURCE;
	radio.settings.brightness = RADIO_FACORY_BRIGHTNESS;
	radio.settings.contrast = RADIO_FACORY_CONTRAST;

	for(i = 0; i < 14; i++) {
		flash_store((char *)empty_text, RADIO_STATION_NAME_STORE_SIZE, RADIO_STATION_NAME_STORE_ADR + RADIO_STATION_NAME_STORE_SIZE * i);
		flash_store(&i, RADIO_STATION_FREQ_STORE_SIZE, RADIO_STATION_FREQ_STORE_ADR + RADIO_STATION_FREQ_STORE_SIZE * i);
	}

	radio_store_settings(1, 1);

	i = 0xAA;

	flash_store(&i, 1, RADIO_INIT_STORE_ADR);

	return 0;
}

//----------------------------------------------------------------------------------------
//
/// \brief Turn radio to standby
//
/// (1)Turn off amplifier\n
/// (2)Turn off si4735\n
/// (3)store actuall freqency\n
/// (4)store actuall volume\n
/// (5)Turn off display\n
/// (6)enable interrupt for left button\n
/// (7)reconfig one timer with ACLK to cause an interrupt all minute\n
/// (8)go to lpm mode where ACLK is active\n
/// (9)wait until button is pressed\n
/// (10)reconfig timer to old state\n
/// (11)enable amplifier with stored values\n
/// (12)enable si4735 with stored values\n
/// (13)turn on display needed to be init\n
/// (14)go back to normal operations
//
/// \param
//
/// \retval uint8_t
//
/// \remarks
//
//----------------------------------------------------------------------------------------

uint8_t radio_stand_by()
{
	tpa2016d2_powermode(TPA2016D2_SHUTDOWN);
	si4735_shutdown();
	//TODO store actuall freqency
	//TODO store actuall volume
	lcd_create_view(0, 0, 0, 0, 2);
	pca9530_set_pwm(PWM_0, 0);
	//TODO enable interrupt for left button
	//TODO reconfig one timer with ACLK to cause an interrupt all minute
	//while(/* TODO checkfor left encoder button interrupt */) {
		_BIS_SR(LPM3_bits + GIE);
	//}
	//TODO go to lpm mode where ACLK is active
	//TODO wait until button is pressed
	//TODO reconfig timer to old state
	tpa2016d2_powermode(TPA2016D2_POWERUP);
	SI4735_INIT();
	radio_volume(radio.settings.volume);
	radio_tune_freq(radio.settings.frequency);
	pca9530_set_pwm(PWM_0, 256-exp_table[radio.settings.brightness * 2]);
	return 0;
}
