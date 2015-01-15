/*
 * LCDDOGMA.h
 *
 *  Created on: 16.12.2013
 *      Author: Alexander
 */

#ifndef LCDDOGMA_H_
#define LCDDOGMA_H_


void i2c_lcd_init();
void i2c_lcd_write_char(unsigned char Symbol);
void i2c_lcd_set_courser(unsigned char X, unsigned char Y);
void i2c_lcd_command(unsigned char Command);
void i2c_lcd_write_string(const char *s, unsigned char n_bytes);
void i2c_lcd_connect();
void i2c_lcd_generatechar(unsigned char code, const unsigned char *data);
void i2c_lcd_bargraph(unsigned char value);
signed char i2c_lcd_create_view(const char *string, unsigned char x_pos, unsigned char y_pos, unsigned char flush_data);
void i2c_lcd_generatebargraph();
void i2c_lcd_brightnes(unsigned int brightnes);


#endif /* LCDDOGMA_H_ */
