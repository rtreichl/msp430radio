/*
 * radio_new.c
 *
 *  Created on: 31.01.2015
 *      Author: Richard
 */

#include <stdint.h>
#include <driver/pca9530.h>
#include <libs/log_exp_table.h>
#include <driver/si4735.h>

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
	volume = ((uint16_t)volume * SI4735_VOLUME_MAX) / 100;
	SI4735_Set_Volume(volume);
	return 0;
}

uint8_t radio_init(void)
{
	//TODO new init structur for init all radio configs based on settings on radio_new.h
	return 0;
}

uint8_t radio_handler(void)
{
	//TODO new radio handler which controll all for radio time and interupt based.
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
/*
uint8_t radio_store_station(uint16_t freq, uint8_t *name)
{
	if(name == 0)
	{
		uint8_t alt_name[8];
		uint8_t pos = 0;
		if(freq < 10000) {
			pos++;
			alt_name[6] = 0;
		}
		for(uint16_t i = freq; i >= 0; i \= 10, pos++) {
			if(i < 100) {
				alt_name[6+pos] = '.';
				pos++;
			}
			alt_name[6+pos] = '0' + i % 10;
		}
	}
	return 0;
}

uint8_t radio_auto_search()
{
	uint8_t resp[8];
	si4735_fm_tune_freq(RADIO_BOT_FREQ);
	do {
		//TODO seek up to next channel
		si4735_fm_seek_start(1);
		//TODO read frequency and channel data and acknolege interrupt.
		si4735_fm_tune_status(0, 1, resp);
		//TODO wait some time for a vaild rds signal else store without
		si4735_get_rds_data();
		/* TODO Description for rds implementation, rds data will always be taken over handler if he get´s valids rds blocks.
		 * The rds_function gives back several stats like rds station name valid, rds programm text valid. Also it will handle
		 * after a radio station name got valid it will not be readed out again. To get a vaild radio station name there should
		 * be two or three time the same name from rds, after this i could be said that the radio station name is valid.
		 * Syncronization of the Clock should be all ways checked for valid this could be happen by checking time difference
		 * from one time to the other. The time difference should be counted after first time was gotten. The gap between the
		 * first and second valid time should be the difference, which was counted during this. For a valid rds text there should
		 * be two times the same. After that the function should be polling for a new radio text indicator.
		 */

		//TODO implement a better way to get rds data with a valid condition

	} while()//TODO do it untill top seek frequency is reached or station storage is full.
	//TODO Display list of founded Stations with a return mark on bottom.
	//TODO Go to first valid Station.
	//TODO mute SI4735 => go to lowest possible frequency => configure valid signal => start seeking up => wait valid channel interrupt => wait for valid rds information only station name => overwrite rom with new channel data => seek up agian
	//																							|																																			|
	//																							--------------------------------------------------------------------------------------------------------------------------------------------- => until highest frequency is reached
	return 0;
}*/
