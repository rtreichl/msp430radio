#ifndef _MENU_H_
#define _MENU_H_

#include <stdint.h>
#include <driver/lcd.h>
#include <menu/lang/language.h>
#include <menu/menu_struct.h>

#define MENU_NO_FUNC_ENTRY 0
#define MENU_FREQ_ENTRY	1
#define MENU_VOL_ENTRY	2
#define MENU_CONT_ENTRY	3
#define MENU_BRIG_ENTRY	4
#define MENU_MAIN_ENTRY 5

uint8_t menu_handler(uint8_t *encoder_2_button, int8_t *encoder_2_count);
uint8_t menu_scroll(uint8_t *encoder_button, int8_t *encoder_count);
uint8_t menu_main(uint8_t *encoder_button, int8_t *encoder_count);

#endif /* _MENU_H_ */
