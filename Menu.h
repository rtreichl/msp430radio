/*
 * Menu.h
 *
 *  Created on: 02.01.2014
 *      Author: Richi
 */

#ifndef MENU_H_
#define MENU_H_

#define MAX_ZERO_LVL 4
#define MAX_FIRST_LVL 6
#define MAX_SECOND_LVL 6
#define MAX_THIRD_LVL 4
#define BLEND_OUT_AFTER	3 //(3-1)*250ms
#define AMPLIFIER_GAIN 15
/*#define BUTTON_PRESS_LONG 0x6C
#define BUTTON_PRESS_SHORT 0x6B
#define BUTTON_FREE 0x66*/

void menu(void);
void null_befor_value(char *string ,char value, unsigned char pos);
void time_date(unsigned char t_hour, unsigned char t_minute, unsigned char t_day, unsigned char t_month, unsigned char t_year, unsigned char update, unsigned char minute_up, char *s_time, char *s_date);
void enter_standby(void);
void exit_standby(void);
char check_for_out_of_range(signed char value,char modulo);

#endif /* MENU_H_ */
