#ifndef _MENU_H_
#define _MENU_H_

#include <stdint.h>
#include <driver/lcd.h>
#include <system/radio.h>
#include <menu/lang/language.h>
#include <menu/menu_struct.h>

#define MENU_NO_FUNC_ENTRY 0
#define MENU_FREQ_ENTRY	1
#define MENU_VOL_ENTRY	2
#define MENU_CONT_ENTRY	3
#define MENU_BRIG_ENTRY	4
#define MENU_MAIN_ENTRY 5
#define RADIO_RDS_VIEW_ENTRY 6
#define RADIO_RSQ_VIEW_ENTRY 7
#define RADIO_PIPTY_VIEW_ENTRY 8
#define AUDIO_POP_ENTRY 9
#define AUDIO_ROCK_ENTRY 10
#define AUDIO_CLASSIC_ENTRY 11
#define AUDIO_JAZZ_ENTRY 12
#define AUDIO_HIPHOP_ENTRY 13
#define AUDIO_NEWS_ENTRY 14
#define SOURCE_AM_ENTRY 15
#define SOURCE_FM_ENTRY 16
#define SOURCE_LINEIN_ENTRY  17





uint8_t menu_handler(uint8_t *encoder_left_button, int8_t *encoder_left_count, uint8_t *encoder_right_button, int8_t *encoder_right_count);
uint8_t menu_scroll(uint8_t value);
uint8_t menu_main(uint8_t *encoder_button, int8_t *encoder_count);
uint8_t menu_scroll_settings(uint8_t value);
uint8_t menu_function(uint8_t *encoder_left_button, int8_t *encoder_left_count, uint8_t *encoder_right_button, int8_t *encoder_right_count);

#endif /* _MENU_H_ */
