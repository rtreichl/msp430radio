/*
 * lcd.c
 *
 *  Created on: 09.02.2015
 *  Author: Richard Treichl
 */

#include <driver/lcd.h>

int8_t lcd_replace_special_letter(int8_t chr);

uint8_t lcd_init(uint8_t contrast)
{

	// Activate PCA9534
	PCA_ON_OFF_SEL &= (~PCA_ON_OFF_PIN);
	PCA_ON_OFF_DIR |= PCA_ON_OFF_PIN;
	PCA_ON_OFF_OUT |= PCA_ON_OFF_PIN;


	// Go in default state
	//pca9534_config(0x7F);

	//i2c_write_var(PCA9534_I2C_ADR, STOP, 2,0x03, 0x00);

	_delay_ms(100);

	// Send reset command
	i2c_write_var(PCA9534_I2C_ADR, STOP, 2, 0x03, 0x13);

	_delay_ms(2);

	// Send 2nd reset command
	i2c_write_var(PCA9534_I2C_ADR, STOP, 2, 0x03, 0x13);

	_delay_ten_us(3);

	// Send 3rd reset command
	i2c_write_var(PCA9534_I2C_ADR, STOP, 2, 0x03, 0x13);

	// Go in default data mode of display
	i2c_write_var(PCA9534_I2C_ADR, STOP, 2, 0x02, 0x12);

	_delay_ten_us(3);

	lcd_command(LCD_FUNCTION_SET | LCD_2_LINE_INST_1);
	lcd_command(LCD_BIAS_SET | LCD_BIAS_1_4 | LCD_3_LINE);
	lcd_command(LCD_POWER_CONTROL | ((0x30 & contrast) >> 4));
	lcd_command(LCD_FOLLOWER_CONTROL | LCD_FOLLOWER_CIRCUT_ON | LCD_AMPLIFFED_RATIO);
	lcd_command(LCD_CONTRAST_SET | (0x0F & contrast));
	lcd_command(LCD_FUNCTION_SET | LCD_INST_0);
	lcd_command(LCD_DISPLAY_CONTROL | LCD_DISPLAY_ON);
	lcd_command(LCD_CLEAR_SCREEN);
	lcd_command(LCD_ENTRY_MODE_SET | LCD_COURSER_INKREMENT);
	lcd_command(LCD_RESET_COURSER);

	//Gernarte GCCR Symbole
	lcd_generatebargraph();

	//Clear LCD and Init view ram
	lcd_create_view(0, 0, 0, 0, 2);

	return 0;
}

uint8_t lcd_contrast(uint8_t contrast)
{
	lcd_command(LCD_FUNCTION_SET | LCD_2_LINE_INST_1);
	lcd_command(LCD_POWER_CONTROL | ((0x30 & contrast) >> 4));
	lcd_command(LCD_CONTRAST_SET | (0x0F & contrast));
	lcd_command(LCD_FUNCTION_SET | LCD_INST_0);

	return 0;
}

uint8_t lcd_write_char(uint8_t symbol)
{

	//symbol = lcd_replace_special_letter(symbol);

	/*i2c_write_var(PCA9534_I2C_ADR, STOP, 1, (((symbol>>4) & 0x0f) | 0x40));
	i2c_write_var(PCA9534_I2C_ADR, STOP, 1, (((symbol>>4) & 0x0f) | 0x50));
	i2c_write_var(PCA9534_I2C_ADR, STOP, 1, ((symbol & 0x0f) | 0x40));
	i2c_write_var(PCA9534_I2C_ADR, STOP, 1, ((symbol & 0x0f) | 0x50));
	i2c_write_var(PCA9534_I2C_ADR, STOP, 1, 0x08);*/

	_delay_ten_us(20);

	i2c_write_var(PCA9534_I2C_ADR, STOP, 5,
			0x00,
			LCD_HIGHER_BYTE(symbol) | LCD_ENABLE_HIGH | LCD_DATA,
			LCD_HIGHER_BYTE(symbol) | LCD_ENABLE_LOW  | LCD_DATA,
			LCD_LOWER_BYTE(symbol)  | LCD_ENABLE_HIGH | LCD_DATA,
			LCD_LOWER_BYTE(symbol)  | LCD_ENABLE_LOW  | LCD_DATA);

	return 0;
}

uint8_t lcd_write_string(const int8_t *str, unsigned char n_bytes)
{
	register uint8_t chr;
	uint8_t i = 0;

	for(i = 0; i < n_bytes; i++)
	{
		chr = *str++;

		switch(chr) {
			case 0xF6:
			case 0x97:
				chr = 0x94; //ö
				break;
			case 0xFC:
			case 0x99:
				chr = 0x81; //ü
				break;
			case 0xE4:
			case 0x91:
				chr = 0x84; //ä
				break;
			case 0xDC:
			case 0xD9:
				chr = 0x9A; //Ü
				break;
			case 0xD6:
			case 0xD7:
				chr = 0x99; //Ö
				break;
			case 0xC4:
			case 0xD1:
				chr = 0x8E; //Ä
				break;
		}

		//chr = lcd_replace_special_letter(chr);

		//i2c_write_var(PCA9534_I2C_ADR, STOP, 5, (((chr>>4) & 0x0f) | 0x40), (((chr>>4) & 0x0f) | 0x50), ((chr& 0x0f) | 0x40), ((chr & 0x0f) | 0x50));

		i2c_write_var(PCA9534_I2C_ADR, STOP, 5,
				0x00,
				LCD_HIGHER_BYTE(chr) | LCD_ENABLE_HIGH | LCD_DATA,
				LCD_HIGHER_BYTE(chr) | LCD_ENABLE_LOW  | LCD_DATA,
				LCD_LOWER_BYTE(chr)  | LCD_ENABLE_HIGH | LCD_DATA,
				LCD_LOWER_BYTE(chr)  | LCD_ENABLE_LOW  | LCD_DATA);
	}

	return 0;
}

uint8_t lcd_command(uint8_t command)
{
	i2c_write_var(PCA9534_I2C_ADR, STOP, 1, ((command>>4) & 0x0f));
	_delay_ms(1);
	i2c_write_var(PCA9534_I2C_ADR, STOP, 1, (((command>>4) & 0x0f) | 0x10));
	_delay_ms(1);
	i2c_write_var(PCA9534_I2C_ADR, STOP, 1, (command & 0x0f));
	_delay_ms(1);
	i2c_write_var(PCA9534_I2C_ADR, STOP, 1, ((command & 0x0f) | 0x10));
	_delay_ms(5);
	/*i2c_write_var(PCA9534_I2C_ADR, STOP, 2, 0x01, LCD_HIGHER_BYTE(command) | LCD_ENABLE_HIGH);
	i2c_write_var(PCA9534_I2C_ADR, STOP, 2, 0x01, LCD_HIGHER_BYTE(command) | LCD_ENABLE_LOW);
	i2c_write_var(PCA9534_I2C_ADR, STOP, 2, 0x01, LCD_LOWER_BYTE(command) | LCD_ENABLE_HIGH);
	i2c_write_var(PCA9534_I2C_ADR, STOP, 2, 0x01, LCD_LOWER_BYTE(command) | LCD_ENABLE_LOW);*/

	return 0;
}

int8_t lcd_replace_special_letter(int8_t chr)
{
	return chr;
}

uint8_t lcd_set_courser(uint8_t x, uint8_t y)
{
	if (x > 15) {
		return 0xFE;
	}

	switch (y) {
		case 1:
			lcd_command(x + LCD_ADRESS_LINE_1);
			break;
		case 2:
			lcd_command(x + LCD_ADRESS_LINE_2);
			break;
		case 3:
			lcd_command(x + LCD_ADRESS_LINE_3);
			break;
		case 4:
			lcd_command(x + LCD_ADRESS_LINE_4);
			break;
		default:
			return 0xFF;
	}
	return 0;
}

uint8_t lcd_generatechar(uint8_t code, const uint8_t *data)
{
	uint8_t i = 0;
    // Startposition des Zeichens einstellen
	lcd_command(LCD_SET_CGADR|(code<<3));

    // Bitmuster übertragen
    for (i = 0; i < 8; i++)
    {
    	lcd_write_char(data[i]);
    }

    return 0;
}

uint8_t lcd_generatebargraph()
{
	const uint8_t chrdata1[8] = {
        0x1F,	//xxxxx
        0x10,	//xoooo
        0x10,   //xoooo
        0x10,   //xoooo
        0x10,   //xoooo
        0x10,   //xoooo
        0x1F,	//xxxxx
        0x00
	};

	const uint8_t chrdata2[8] = {
		0x1F,	//xxxxx
		0x18,	//xxooo
		0x18,   //xxooo
		0x18,   //xxooo
		0x18,   //xxooo
		0x18,   //xxooo
        0x1F,	//xxxxx
        0x00
	};

	const uint8_t chrdata3[8] = {
		0x1F,	//xxxxx
		0x1C,	//xxxoo
        0x1C,   //xxxoo
        0x1C,   //xxxoo
        0x1C,   //xxxoo
        0x1C,   //xxxoo
        0x1F,	//xxxxx
        0x00
	};

	const uint8_t chrdata4[8] = {
		0x1F,	//xxxxx
		0x1E,	//xxxxo
        0x1E,   //xxxxo
        0x1E,   //xxxxo
        0x1E,   //xxxxo
        0x1E,   //xxxxo
        0x1F,	//xxxxx
        0x00
	};

	const uint8_t chrdata5[8] = {
		0x1F,	//xxxxx
		0x1F,	//xxxxx
		0x1F,   //xxxxx
        0x1F,   //xxxxx
        0x1F,   //xxxxx
        0x1F,   //xxxxx
        0x1F,	//xxxxx
        0x00
	};

	const uint8_t chrdata0[8] = {
		0x1F,	//xxxxx
		0x00,	//ooooo
		0x00,   //ooooo
		0x00,   //ooooo
		0x00,   //ooooo
		0x00,   //ooooo
        0x1F,	//xxxxx
        0x00
	};

	const uint8_t chrdata6[8] = {
		0x0E,	//oxxxo
		0x15,	//xoxox
		0x15,   //xoxox
		0x17,   //xoxxx
		0x11,   //oooox
		0x11,   //xooox
        0x0E,	//oxxxo
        0x00
	};

	const uint8_t chrdata7[8] = {
		0x03,	//oooxx
		0x05,	//ooxox
		0x19,   //xxoox
		0x11,   //xxoox
		0x19,   //xxoox
		0x05,   //ooxox
        0x03,	//oooxx
        0x00
	};

	lcd_generatechar(0,chrdata0);
	lcd_generatechar(1,chrdata1);
	lcd_generatechar(2,chrdata2);
	lcd_generatechar(3,chrdata3);
	lcd_generatechar(4,chrdata4);
	lcd_generatechar(5,chrdata5);
	lcd_generatechar(6,chrdata6);
	lcd_generatechar(7,chrdata7);

	return 0;
}

uint8_t lcd_create_view(const char *str, uint8_t x, uint8_t y, uint8_t num, uint8_t flush)
{
	static int8_t lcd_view[49];

	uint8_t	i,
			pos;
	/* invalid line position */
	if (x > 15) {
		return 0xFE;
	}

	/* invalid row position */
	if (y > 2) {
		return 0xFF;
	}

	pos = x + y * 16;

	if(num == 0) {
		num = 48;
	}

	/* if string is empty determinat string on given position */
	if(*str == 0 && num == 48) {
		lcd_view[pos] = 0;
	}
	else {
		/* copy string on given position */
		while(*str != 0 && pos < 48 && str != 0 && num > 0) {//&& num > 0) {
			lcd_view[pos++] = *(str++);
			num--;
		}
	}

	/* Normal flush with over writing lcd_view with blanks */
	if(flush == 1) {
		lcd_set_courser(0,1);
		lcd_write_string(lcd_view,48);
		for(i = 48; i-- > 0;)	{
			lcd_view[i] = ' ';
		}
		lcd_view[48] = 0;
	}

	/* Init lcd_view with blanks */
	if(flush == 2) {
		for(i = 48; i-- > 0;) {
			lcd_view[i] = 0x20;
		}
		lcd_view[48] = 0;
		lcd_set_courser(0,1);
		lcd_write_string(lcd_view,48);
	}

	return 0;
}
