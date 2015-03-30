/*
 * lcd.h
 *
 *  Created on: 16.12.2013
 *      Author: Alexander
 */

#ifndef LCD_H_
#define LCD_H_

#include <stdint.h>
#include <driver/pca9534.h>
#include <driver/include/lcd_commands.h>
#include <settings/radio_pin_mapping.h>
#include <driver/timer.h>

/* Based on pin mapping of PCA9534 */

#define LCD_ENABLE_HIGH				0x10
#define LCD_ENABLE_LOW				0x00
#define LCD_READ					0x20
#define LCD_WRITE					0x00
#define LCD_DATA					0x40
#define LCD_CMD						0x00

uint8_t lcd_init(uint8_t contrast);
uint8_t lcd_write_char(uint8_t chr);
uint8_t lcd_set_courser(uint8_t x, uint8_t y);
uint8_t lcd_command(uint8_t cmd);
uint8_t lcd_write_string(const int8_t *str, uint8_t n_bytes);
uint8_t lcd_connect();
uint8_t lcd_generatechar(uint8_t code, const uint8_t *data);
uint8_t lcd_bargraph(uint8_t value);
uint8_t lcd_create_view(const char *str, uint8_t x, uint8_t y, uint8_t flush);
uint8_t lcd_contrast(uint8_t contrast);


#endif /* LCD_H_ */
