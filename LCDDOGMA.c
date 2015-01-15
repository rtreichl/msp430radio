/*
 * LCDDOGMA.c
 *
 *  Created on: 16.12.2013
 *      Author: Alexander
 */

/*
 * I2C_Display.c
 *
 * Created: 01.09.2013 09:39:26
 *  Author: Richi
 */

#include <stdlib.h>
#include <stdio.h>
#include <msp430g2553.h>
#include "MSP430G2553_USCI_I2C.h"
#include "LCDDOGMA.h"
#include "Timer.h"


#define I2C_LCD_ADRESS				0x20
#define SET_EXPANDER_AS_OUTPUT		0x02

#define I2C_LCD_SHIFT_RIGHT			0x1C
#define I2C_LCD_SHIFT_LEFT			0x18
#define I2C_LCD_RESET_COURSER		0x02
#define I2C_LCD_ADRESS_LINE_1		0x80
#define I2C_LCD_ADRESS_LINE_2		0x90
#define I2C_LCD_ADRESS_LINE_3		0xA0
#define I2C_LCD_ADRESS_LINE_4		0xD4
#define I2C_LCD_CLEAR_SCREEN		0x01
#define I2C_LCD_ENTRY_MODE_SET		0x04
#define I2C_LCD_COURSER_INKREMENT	0x02
#define I2C_LCD_DISPLAY_CONTE_MOVE	0x01
#define	I2C_LCD_DISPLAY_CONTROL		0x08
#define	I2C_LCD_DISPLAY_ON			0x04
#define	I2C_LCD_COURSER_ON			0x02
#define I2C_LCD_COURSER_BLINK		0x01
#define I2C_LCD_FUNCTION_SET		0x20
#define I2C_LCD_8BTI_MODE			0x10
#define I2C_LCD_5x10_FONT			0x04
#define I2C_LCD_2_LINE_INST_1		0x09
#define I2C_LCD_INST_0				0x08
#define I2C_LCD_SET_CGADR			0x40
#define I2C_LCD_BIAS_SET			0x14
#define I2C_LCD_BIAS_1_4			0x08
#define I2C_LCD_3_LINE				0x01
#define I2C_LCD_POWER_CONTROL		0x50
#define I2C_LCD_ICON_ON				0x08
#define I2C_LCD_BOOSTER_ON			0x04
#define I2C_LCD_CONTRAST_1			0x00
#define I2C_LCD_FOLLOWER_CONTROL	0x60
#define I2C_LCD_FOLLOWER_CIRCUT_ON	0x08
#define I2C_LCD_AMPLIFFED_RATIO		0x04
#define I2C_LCD_CONTRAST_SET		0x70
#define I2C_LCD_CONTRAST_2			0x0C


#define I2C_LCD_PORT_DIR			P3DIR
#define I2C_LCD_PORT_OUT			P3OUT
#define I2C_LCD_PORT_PIN			BIT1
#define I2C_LCD_PORT_SEL			P3SEL
#define I2C_LCD_PORT_SEL2			P3SEL2


#define I2C_LCD_BAUDRATE			40

void i2c_lcd_brightnes(unsigned int brightnes)
{
	//I2C_LCD_PORT_SEL |= I2C_LCD_PORT_PIN;
	I2C_LCD_PORT_DIR |= I2C_LCD_PORT_PIN;
	I2C_LCD_PORT_OUT |= I2C_LCD_PORT_PIN;
	//TA1CCR0 = brightnes;                       // PWM Period
	//TA1CCTL1 = OUTMOD_6;                       // CCR1 reset/set
	//TA1CCR1 = 384;                             // CCR1 PWM duty cycle
	//TA1CTL = TASSEL_1 + MC_3;                  // ACLK, up mode
}

void i2c_lcd_init()
{
	char Kommando [4];

	USCI_I2C_INIT (0x20,I2C_LCD_BAUDRATE);

	sprintf(Kommando, "%c%c", 0x03,0x00);
	USCI_I2C_WRITE1(2, Kommando);

	_delay_ms(40);
	USCI_I2C_INIT (0x20,I2C_LCD_BAUDRATE);

	//sprintf(Kommando, "%c", 0x01);
	//USCI_I2C_WRITE1(1, Kommando);

	sprintf(Kommando, "%c%c%c",0x01, 0x03, 0x13);
	USCI_I2C_WRITE1(3, Kommando);

	_delay_ms(2);

	sprintf(Kommando, "%c%c%c",0x01, 0x03, 0x13);
	USCI_I2C_WRITE1(3, Kommando);

	_delay_us(30);

	sprintf(Kommando, "%c%c%c",0x01, 0x03, 0x13);
	USCI_I2C_WRITE1(3, Kommando);

	sprintf(Kommando, "%c%c%c",0x01, 0x02, 0x12);
	USCI_I2C_WRITE1(3, Kommando);

	_delay_us(30);

	i2c_lcd_command(I2C_LCD_FUNCTION_SET | I2C_LCD_2_LINE_INST_1);
	i2c_lcd_command(I2C_LCD_BIAS_SET | I2C_LCD_BIAS_1_4 | I2C_LCD_3_LINE);
	i2c_lcd_command(I2C_LCD_POWER_CONTROL | I2C_LCD_CONTRAST_1);
	i2c_lcd_command(I2C_LCD_FOLLOWER_CONTROL | I2C_LCD_FOLLOWER_CIRCUT_ON | I2C_LCD_AMPLIFFED_RATIO);
	i2c_lcd_command(I2C_LCD_CONTRAST_SET | I2C_LCD_CONTRAST_2);
	i2c_lcd_command(I2C_LCD_FUNCTION_SET | I2C_LCD_INST_0);
	i2c_lcd_command(I2C_LCD_DISPLAY_CONTROL | I2C_LCD_DISPLAY_ON);
	i2c_lcd_command(I2C_LCD_CLEAR_SCREEN);
	i2c_lcd_command(I2C_LCD_ENTRY_MODE_SET | I2C_LCD_COURSER_INKREMENT);
	i2c_lcd_command(I2C_LCD_RESET_COURSER);
}
void i2c_lcd_write_char(unsigned char Symbol)
{
	char Kommando [3];
	USCI_I2C_INIT (0x20,I2C_LCD_BAUDRATE);


	sprintf(Kommando, "%c%c", 0x01, (((Symbol>>4) & 0x0f) | 0x40)/*, (((Symbol>>4) & 0x0f) | 0x50), ((Symbol & 0x0f) | 0x40), ((Symbol & 0x0f) | 0x50), 0x08*/);
	USCI_I2C_WRITE1(2, Kommando);
	sprintf(Kommando, "%c%c", 0x01, (((Symbol>>4) & 0x0f) | 0x50));
	USCI_I2C_WRITE1(2, Kommando);
	sprintf(Kommando, "%c%c", 0x01, ((Symbol & 0x0f) | 0x40));
	USCI_I2C_WRITE1(2, Kommando);
	sprintf(Kommando, "%c%c", 0x01, ((Symbol & 0x0f) | 0x50));
	USCI_I2C_WRITE1(2, Kommando);
	sprintf(Kommando, "%c%c", 0x01, 0x08);
	USCI_I2C_WRITE1(2, Kommando);
	_delay_us(200);
}

void i2c_lcd_write_string(const char *s, unsigned char n_bytes)
{
	char Kommando [3];
	register char Symbol;
	char i = 0;
	USCI_I2C_INIT (0x20,I2C_LCD_BAUDRATE);

	for(i = 0; i < n_bytes; i++)
	//while ( ()
	{
		Symbol = *s++;
		switch(Symbol)
		{
			case 0x97:
				Symbol = 0x94; //ö
				break;
			case 0x99:
				Symbol = 0x81; //ü
				break;
			case 0x91:
				Symbol = 0x84; //ä
				break;
			case 0xD9:
				Symbol = 0x9A; //Ü
				break;
			case 0xD7:
				Symbol = 0x99; //Ö
				break;
			case 0xD1:
				Symbol = 0x8E; //Ä
				break;
		}
		sprintf(Kommando, "%c%c", 0x01, (((Symbol>>4) & 0x0f) | 0x40)/*, (((Symbol>>4) & 0x0f) | 0x50), ((Symbol & 0x0f) | 0x40), ((Symbol & 0x0f) | 0x50), 0x08*/);
		USCI_I2C_WRITE1(2, Kommando);                        // Write M
		sprintf(Kommando, "%c%c", 0x01, (((Symbol>>4) & 0x0f) | 0x50));
		USCI_I2C_WRITE1(2, Kommando);
		sprintf(Kommando, "%c%c", 0x01, ((Symbol & 0x0f) | 0x40));
		USCI_I2C_WRITE1(2, Kommando);
		sprintf(Kommando, "%c%c", 0x01, ((Symbol & 0x0f) | 0x50));
		USCI_I2C_WRITE1(2, Kommando);
		sprintf(Kommando, "%c%c",0x01, 0x08);
		USCI_I2C_WRITE1(2, Kommando);
		_delay_us(100);
	}
}

void i2c_lcd_command(unsigned char Command)
{
	char Kommando [3];
	USCI_I2C_INIT (0x20,40);
	//sprintf(Kommando, "%c", 0x01);
	//USCI_I2C_WRITE1(1, Kommando);

	sprintf(Kommando, "%c%c", 0x01, ((Command>>4) & 0x0f)/*, (((Command>>4) & 0x0f) | 0x10), (Command & 0x0f), ((Command & 0x0f) | 0x10)*/);
	USCI_I2C_WRITE1(2, Kommando);

	sprintf(Kommando, "%c%c", 0x01, (((Command>>4) & 0x0f) | 0x10));
	USCI_I2C_WRITE1(2, Kommando);

	sprintf(Kommando, "%c%c", 0x01, (Command & 0x0f));
	USCI_I2C_WRITE1(2, Kommando);

	sprintf(Kommando, "%c%c", 0x01, ((Command & 0x0f) | 0x10));
	USCI_I2C_WRITE1(2, Kommando);
	_delay_ms(5);
}

void i2c_lcd_set_courser(unsigned char X, unsigned char Y)
{
	switch (Y)
	{
	case 1:
		i2c_lcd_command(X+I2C_LCD_ADRESS_LINE_1);
		break;
	case 2:
		i2c_lcd_command(X+I2C_LCD_ADRESS_LINE_2);
		break;
	case 3:
		i2c_lcd_command(X+I2C_LCD_ADRESS_LINE_3);
		break;
	case 4:
		i2c_lcd_command(X+I2C_LCD_ADRESS_LINE_4);
		break;
	}
}

void i2c_lcd_generatechar(unsigned char code, const unsigned char *data)
{
	char i = 0;
    // Startposition des Zeichens einstellen
	i2c_lcd_command(I2C_LCD_SET_CGADR|(code<<3));

    // Bitmuster übertragen
    for (i=0; i<8; i++)
    {
    	i2c_lcd_write_char(data[i]);
    }
}

void i2c_lcd_generatebargraph()
{
	unsigned char chrdata1[8] = {
        0x1F,	//xxxxx
        0x10,	//xoooo
        0x10,   //xoooo
        0x10,   //xoooo
        0x10,   //xoooo
        0x10,   //xoooo
        0x1F,	//xxxxx
        0x00
	};

	unsigned char chrdata2[8] = {
		0x1F,	//xxxxx
		0x18,	//xxooo
		0x18,   //xxooo
		0x18,   //xxooo
		0x18,   //xxooo
		0x18,   //xxooo
        0x1F,	//xxxxx
        0x00
	};

	unsigned char chrdata3[8] = {
		0x1F,	//xxxxx
		0x1C,	//xxxoo
        0x1C,   //xxxoo
        0x1C,   //xxxoo
        0x1C,   //xxxoo
        0x1C,   //xxxoo
        0x1F,	//xxxxx
        0x00
	};

	unsigned char chrdata4[8] = {
		0x1F,	//xxxxx
		0x1E,	//xxxxo
        0x1E,   //xxxxo
        0x1E,   //xxxxo
        0x1E,   //xxxxo
        0x1E,   //xxxxo
        0x1F,	//xxxxx
        0x00
	};

	unsigned char chrdata5[8] = {
		0x1F,	//xxxxx
		0x1F,	//xxxxx
		0x1F,   //xxxxx
        0x1F,   //xxxxx
        0x1F,   //xxxxx
        0x1F,   //xxxxx
        0x1F,	//xxxxx
        0x00
	};

	unsigned char chrdata0[8] = {
		0x1F,	//xxxxx
		0x00,	//ooooo
		0x00,   //ooooo
		0x00,   //ooooo
		0x00,   //ooooo
		0x00,   //ooooo
        0x1F,	//xxxxx
        0x00
	};

	unsigned char chrdata6[8] = {
		0x0E,	//oxxxo
		0x15,	//xoxox
		0x15,   //xoxox
		0x17,   //xoxxx
		0x11,   //oooox
		0x11,   //xooox
        0x0E,	//oxxxo
        0x00
	};

	unsigned char chrdata7[8] = {
		0x03,	//oooxx
		0x05,	//ooxox
		0x19,   //xxoox
		0x11,   //xxoox
		0x19,   //xxoox
		0x05,   //ooxox
        0x03,	//oooxx
        0x00
	};

	i2c_lcd_generatechar(0,chrdata0);
	i2c_lcd_generatechar(1,chrdata1);
	i2c_lcd_generatechar(2,chrdata2);
	i2c_lcd_generatechar(3,chrdata3);
	i2c_lcd_generatechar(4,chrdata4);
	i2c_lcd_generatechar(5,chrdata5);
	i2c_lcd_generatechar(6,chrdata6);
	i2c_lcd_generatechar(7,chrdata7);
}

signed char i2c_lcd_create_view(const char *string, unsigned char x_pos, unsigned char y_pos, unsigned char flush_data)
{
	static char display_lane[49];

	unsigned char 	i = 0,
					string_pos = 0;

	string_pos = x_pos + y_pos*16;

	if(string_pos >= 48)
		return -1;

	if(*string == 0 && flush_data != 1)
	{
		display_lane[string_pos] = 0;
	}
	while(*string != 0 && string_pos < 48)
	{
		display_lane[string_pos++] = *(string++);
	}


	if(flush_data == 1)
	{
		i2c_lcd_set_courser(0,1);
		i2c_lcd_write_string(display_lane,48);
		for(i = 0; i < 48; i++)
		{
			display_lane[i] = ' ';
		}
		display_lane[48] = 0;
	}

	return 0;
}

void i2c_lcd_bargraph(unsigned char value)
{
	char i = 0;
	char string[2];
	i2c_lcd_create_view("- ",0,2,0);
	for(i = 1;i <= value/5 && i <= 12;i++)
		i2c_lcd_create_view("\5",1+i,2,0);
	if(i<=12)
	{
		sprintf(string,"%c",value%5);
		i2c_lcd_create_view(string,1+i,2,0);
	}
	i++;
	for(i=i;i<=12;i++)
		i2c_lcd_create_view("\0",1+i,2,0);
	i2c_lcd_create_view(" +",14,2,0);
}
