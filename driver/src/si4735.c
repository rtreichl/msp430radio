/*
 * SI4735.c
 *
 *  Created on: 04.07.2013
 *      Author: Alexander Raskopf
 *      Status: Nach dem Test
 *
 *   Kommentar: In bearbeitung: die Frequenz soll über FM_Tune_Status abgefragt und im Flash gespeichert werden
 *   							es soll auch die zuletzt verwendete Lautstärke gespeichert werden
 */
#include <driver/si4735.h>
#include <si4735_cmd_prop.h>


#define SI4735_volume 20 		// Maximalwert für die Lautstärke
#define SI4735_volume_start  20 // Startlautstärke

void SI4735_Set_Volume (signed char Volume) //setzt Volume als die Lautstärke
{
	//char Command[7];
	//char response = 0;
	//sprintf(Command, "%c%c%c%c%c%c", 0x12,0x00,0x40,0x00,0x00,Volume);
	//I2C_write_and_read(6,Command,1,&response);
	i2c_write_var(I2C_SI4735, STOP, 6, 0x12, 0x00, 0x40, 0x00, 0x00, Volume);
}


void SI4735_Fm_Tune_Freq_2 (unsigned int Frequenz) //setzt den FM Empfang und legt die Frequenz zwischen 64 - 108 MHz in 10 kHz einheiten fest
{
	i2c_write_var(I2C_SI4735, STOP, 5, 0x20, 0x01, (0xFF & (Frequenz >> 8)), (0xFF & Frequenz), 0x00);
 	_delay_ms(1);
}

/*void SI4735_FM_RDS_INT_SOURCE() //RDS interrupt Quelle einstellen
{
	char Command[7];
	sprintf(Command, "%c%c%c%c%c%c", 0x12,0x00,0x15,0x00,0x00,0x01);
	USCI_I2C_WRITE1(6,Command);
}*/

void SI4735_Power_Up (void)	//einschalten des Si4735 aktivieren des Analogen Audio Ausgangs
{
	i2c_write_var(I2C_SI4735, STOP, 3, 0x01, 0xD0, 0x05);
}

void SI4735_Power_Down (void)	//einschalten des Si4735 aktivieren des Analogen Audio Ausgangs
{
	i2c_write_var(I2C_SI4735, STOP, 1, 0x11);
}

/*void SI4735_Refclk_Freq (void)	//aktivieren und festlegen der referenc clock frequency 32,768 kHz
{
	char Command[7];
	sprintf(Command, "%c%c%c%c%c%c", 0x12,0x00,0x02,0x01,0x7E,0xF4);
	USCI_I2C_WRITE1(6, Command);
}

void SI4735_RX_Volume (void)	//setzt den Maximalwert für die Lautstärke
{
	char Command[7];
	sprintf(Command, "%c%c%c%c%c%c", 0x12,0x00,0x40,0x00,0x00,SI4735_volume);
	USCI_I2C_WRITE1(6, Command);
}*/

/*void SI4735_Fm_Deemphasis (void)	//entzerrt die Empfangene Frequenz, wert für Europa, Australien und Japan beträgt 50µs
{
	char Command[7];
	sprintf(Command, "%c%c%c%c%c%c", 0x12,0x00,0x11,0x00,0x00,0x01);
	USCI_I2C_WRITE1(6, Command);
}*/

void SI4735_Rx_Hard_Mute (void)	//setzt den Audio Ausgang auf stumm, L und R Audio Ausgang könnten selbständig auf stumm geschaltet sein
{
	i2c_write_var(I2C_SI4735, STOP, 6, 0x12, 0x00, 0x40, 0x01, 0x00, 0x00);
}

/*void SI4735_Fm_Seek_Band_Bottom (void)	//legt die untere Grenze für die Frequenzsuche fest
{
	char Command[7];
	sprintf(Command, "%c%c%c%c%c%c", 0x12,0x00,0x14,0x00,0x22,0x6A);
	USCI_I2C_WRITE2(I2C_SI4735, STOP, 6, Command);
}

void SI4735_Fm_Seek_Band_Top (void)	//legt die obere Grenze für die Frequenzsuche fest
{
	char Command[7];
	sprintf(Command, "%c%c%c%c%c%c", 0x12,0x00,0x14,0x01,0x2A,0x26);
	USCI_I2C_WRITE1(6, Command);
}

void SI4735_Fm_Tune_Freq (void)	//setzt den FM Empfang und legt die Frequenz zwischen 64 - 108 MHz in 10 kHz einheiten fest
{
	char Command[7];
	sprintf(Command, "%c%c%c%c%c", 0x20,0x00,0x2A,0x12,0x00);
	USCI_I2C_WRITE1(5, Command);
}

void SI4735_Fm_Tune_Status (void)	//gibt den Aktuellen Wert der Frequenz oder Suchlauf zurück, ist noch in arbeit
{
	char Command[3];
	char response[8];
	//sprintf(Command, "%c%c", 0x22,0x00);
	//I2C_write_and_read(2,Command,8,response);
	_delay_ms(1);
	USCI_I2C_WRITE2(I2C_SI4735, REPT, 2, 0x22, 0x00);
	//_delay_us(100);
	USCI_I2C_READ(I2C_SI4735, STOP, 8, response);
    _delay_ms(1);
}

void SI4735_Start_Volume (void)	//setzt den Startwert für die Lautstärke
{
	char Command[7];
	if(SI4735_volume==63)
	{
		sprintf(Command, "%c%c%c%c%c%c", 0x12,0x00,0x40,0x00,0x00,SI4735_volume_start);
		USCI_I2C_WRITE1(6, Command);
	}
}
*/

uint8_t si4735_fm_rds_status(uint8_t statusonly, uint8_t mtfifo, uint8_t intack)
{
	uint8_t resp[13];
	FM_RDS_STATUS_ARG1_STC arg1 = { .STATUSONLY = statusonly, .MTFIFO = mtfifo, .INTACK = intack};

	i2c_write_var(I2C_SI4735, REPT, 2, FM_RDS_STATUS, arg1.byte);
	i2c_read(I2C_SI4735, STOP, 13, resp);

	return resp[0];
}

uint8_t si4735_fm_rsq_status(uint8_t intack)
{
	uint8_t resp[8];
	FM_RSQ_STATUS_ARG1_STC arg1 = { .INTACK = intack};

	i2c_write_var(I2C_SI4735, REPT, 2, FM_RSQ_STATUS, arg1.byte);
	i2c_read(I2C_SI4735, STOP, 8, resp);

	return resp[0];
}

uint8_t si4735_fm_tune_status(uint8_t cancel, uint8_t intack)
{
	uint8_t resp[8];
	FM_TUNE_STATUS_ARG1_STC arg1 = { .CANCEL = cancel, .INTACK = intack};

	i2c_write_var(I2C_SI4735, REPT, 2, FM_TUNE_STATUS, arg1.byte);
	i2c_read(I2C_SI4735, STOP, 8, resp);

	return resp[0];
}

uint8_t si4735_fm_tune_freq(uint16_t frequency)
{
	INT_STATUS status;
	FM_TUNE_FREQ_ARG1_STC arg1 = { .FREEZE = 0, .FAST = 0};

	i2c_write_var(I2C_SI4735, REPT, 5, FM_TUNE_FREQ, arg1.byte, HB(frequency), LB(frequency), 0x00);
	i2c_read(I2C_SI4735, STOP, 1, &status.byte);

	return status.byte;
}

uint8_t si4735_get_int_status()
{
	INT_STATUS status;
	//TODO check int pin from SI4735 via interrupt routin if interrupt availible read.
	i2c_write_var(I2C_SI4735, REPT, 1, GET_INT_STATUS);
	i2c_read(I2C_SI4735, STOP, 1, &status.byte);

	return status.byte;
}

uint16_t si4735_get_property(uint16_t property, uint16_t *data)
{
	uint8_t resp[4];

	i2c_write_var(I2C_SI4735, REPT, 6, SET_PROPERTY, 0x00, HB(property), LB(property));
	i2c_read(I2C_SI4735, STOP, 4, resp);

	*data = ((resp[2] << 8) + (resp[3]));

	return resp[0];
}

uint8_t si4735_set_property( uint16_t property, uint16_t data)
{
	INT_STATUS status;
	i2c_write_var(I2C_SI4735, STOP, 5, SET_PROPERTY, 0x00, HB(property), LB(property), HB(data), LB(data));
	i2c_read(I2C_SI4735, STOP, 1, &status.byte);

	return status.byte;
	//TODO wait until command is proceed.
}

uint8_t si4735_fm_seek_start(uint8_t up_down)	//Frequensuchlauf für höhere Frequenzen
{
	INT_STATUS status;
	FM_SEEK_START_ARG1_STC arg1 = {.SEEKUP = up_down, .WRAP = 1};
	//TODO mute audio output
	i2c_write_var(I2C_SI4735, REPT, 2, FM_SEEK_START, arg1.byte);
	i2c_read(I2C_SI4735, STOP, 1, &status.byte);
	//TODO wait until seek stopped on valid freqency
	//TODO unmute audio output
	return status.byte;
}

void SI4735_INIT(void)	// Enthält alle für den Start benötigten Parameter
{
   //SEN
	SI_EN_DIR = SI_EN_PIN;
	SI_EN_OUT = SI_EN_PIN;
	//P3DIR |=   BIT4;
	//P3OUT &=~  BIT4;

	//P2DIR |= BIT4;

	// Clear Reset

	//SI_RST_DIR |= SI_RST_PIN;
	//SI_RST_OUT &= ~SI_RST_PIN;
	P2DIR |=  BIT3;
	P2OUT &=~  BIT3;

	//_delay_ms(1);
	// Set Reset
	//SI_RST_OUT |= SI_RST_PIN;
	P2OUT |=  BIT3;		// Ausgang auf low gesetzt

	//_delay_ms(1);

	//GPO2/INT
	//SI_INT_REN |= SI_INT_PIN;
	//SI_INT_OUT |= SI_INT_PIN;
	P2REN |= BIT4;
	P2OUT |= BIT4;

	_delay_ms(100);

	/* Power_up FM mode */
	i2c_write_var(I2C_SI4735, STOP, 3,  0x01, 0xD0, 0x05);
	while((P2IN & BIT4));
	//_delay_ms(20);

	//__delay_cycles(8000);

	/*activate and setting refclk to 32.768 kHz */
	i2c_write_var(I2C_SI4735, STOP, 6, 0x12, 0x00, 0x02, 0x01, 0x80, 0x00);
	while((P2IN & BIT4));

	/* setup prescaler to 1 */
	i2c_write_var(I2C_SI4735, STOP, 6, 0x12, 0x00, 0x02, 0x02, 0x00, 0x01);
	while((P2IN & BIT4));

	/* setting deemphasis to 50µs standart for europe */
	i2c_write_var(I2C_SI4735, STOP, 6, 0x12, 0x00, 0x11, 0x00, 0x00, 0x01);
	while((P2IN & BIT4));

	/* configure top range for seeking */
	i2c_write_var(I2C_SI4735, STOP, 6,  0x12, 0x00, 0x14, 0x00, 0x22, 0x6A);
	while((P2IN & BIT4));

	/* configure bottom range for seeking */
	i2c_write_var(I2C_SI4735, STOP, 6, 0x12, 0x00, 0x14, 0x01, 0x2A, 0x26);
	while((P2IN & BIT4));

	/* setup RDS interrupt sources */
	i2c_write_var(I2C_SI4735, STOP, 6, 0x12, 0x00, 0x15, 0x00, 0x00, 0x01);
	while((P2IN & BIT4));

	/* setup RDS FIFO count */
	i2c_write_var(I2C_SI4735, STOP, 6, 0x12, 0x00, 0x15, 0x01, 0x00, 0x04);
	while((P2IN & BIT4));

	/* config RDS */
	i2c_write_var(I2C_SI4735, STOP, 6, 0x12, 0x00, 0x15, 0x02, 0xAA, 0x01);
	while((P2IN & BIT4));

	_delay_ms(1);

	/* Tune to frequency 107,7 */
	i2c_write_var(I2C_SI4735, STOP, 5, 0x20, 0x01, 0x2A, 0x12, 0x00);
	while((P2IN & BIT4));

	/* Enable radio audio output with standart value */
	i2c_write_var(I2C_SI4735, STOP, 6, 0x12, 0x00, 0x40, 0x00, 0x00, SI4735_volume);
	while((P2IN & BIT4));
}
