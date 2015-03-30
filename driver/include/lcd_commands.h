/*
 * lcd_commands.h
 *
 *  Created on: 09.02.2015
 *      Author: Richard Treichl
 */

#ifndef LCD_CMD_H_
#define LCD_CMD_H_

#define LCD_SHIFT_RIGHT			0x1C
#define LCD_SHIFT_LEFT			0x18
#define LCD_RESET_COURSER		0x02
#define LCD_ADRESS_LINE_1		0x80
#define LCD_ADRESS_LINE_2		0x90
#define LCD_ADRESS_LINE_3		0xA0
#define LCD_ADRESS_LINE_4		0xD4
#define LCD_CLEAR_SCREEN		0x01
#define LCD_ENTRY_MODE_SET		0x04
#define LCD_COURSER_INKREMENT	0x02
#define LCD_DISPLAY_CONTE_MOVE	0x01
#define	LCD_DISPLAY_CONTROL		0x08
#define	LCD_DISPLAY_ON			0x04
#define	LCD_COURSER_ON			0x02
#define LCD_COURSER_BLINK		0x01
#define LCD_FUNCTION_SET		0x20
#define LCD_8BTI_MODE			0x10
#define LCD_5x10_FONT			0x04
#define LCD_2_LINE_INST_1		0x09
#define LCD_INST_0				0x08
#define LCD_SET_CGADR			0x40
#define LCD_BIAS_SET			0x14
#define LCD_BIAS_1_4			0x08
#define LCD_3_LINE				0x01
#define LCD_POWER_CONTROL		0x50
#define LCD_ICON_ON				0x08
#define LCD_BOOSTER_ON			0x04
#define LCD_CONTRAST_1			0x00
#define LCD_FOLLOWER_CONTROL	0x60
#define LCD_FOLLOWER_CIRCUT_ON	0x08
#define LCD_AMPLIFFED_RATIO		0x04
#define LCD_CONTRAST_SET		0x70
#define LCD_CONTRAST_2			0x0C

#define LCD_LOWER_BYTE(byte)		(byte & 0x0f)
#define LCD_HIGHER_BYTE(byte)		((byte>>4) & 0x0f)

#endif /* LCD_CMD_H_ */
