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
	pca9530_set_pwm(PWM_0, exp_table[brightness]);
	return 0;
}

uint8_t radio_contrast(uint8_t contrast)
{
	//TODO implement a function on LCD for setting contrast
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
