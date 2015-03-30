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
#include <settings/radio_configuration.h>
#include <driver/external_interrupthandler.h>

volatile uint8_t si_interrupt;

void si4735_get_interrupt(uint8_t int_number);

#define SI4735_volume 5 		// Maximalwert für die Lautstärke
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

uint8_t si4735_fm_tune_status(uint8_t cancel, uint8_t intack, uint8_t *resp)
{
	FM_TUNE_STATUS_ARG1_STC arg1 = { .CANCEL = cancel, .INTACK = intack};

	i2c_write_var(I2C_SI4735, REPT, 2, FM_TUNE_STATUS, arg1.byte);
	si4735_get_interrupt(8);
	i2c_read(I2C_SI4735, STOP, 8, resp);

	return resp[0];
}

uint8_t si4735_fm_tune_freq(uint16_t frequency)
{
	INT_STATUS status;
	FM_TUNE_FREQ_ARG1_STC arg1 = { .FREEZE = 0, .FAST = 1};

	i2c_write_var(I2C_SI4735, STOP, 5, FM_TUNE_FREQ, arg1.byte, HB(frequency), LB(frequency), 0x00);
	si4735_get_interrupt(7);
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

	i2c_write_var(I2C_SI4735, STOP, 6, SET_PROPERTY, 0x00, HB(property), LB(property));
	i2c_read(I2C_SI4735, STOP, 4, resp);

	*data = ((resp[2] << 8) + (resp[3]));

	return resp[0];
}

uint8_t si4735_set_property( uint16_t property, uint16_t data)
{
	//INT_STATUS status;
	i2c_write_var(I2C_SI4735, STOP, 6, SET_PROPERTY, 0x00, HB(property), LB(property), HB(data), LB(data));
	si4735_get_interrupt(8);

	return 0;
	//TODO wait until command is proceed.
}

uint8_t si4735_fm_seek_start(uint8_t up_down)	//Frequensuchlauf für höhere Frequenzen
{
	INT_STATUS status;
	FM_SEEK_START_ARG1_STC arg1 = {.SEEKUP = 0, .WRAP = 1};

	arg1.SEEKUP = up_down;
	i2c_write_var(I2C_SI4735, STOP, 2, FM_SEEK_START, arg1.byte);
	si4735_get_interrupt(7);
	i2c_read(I2C_SI4735, STOP, 1, &status.byte);

	return status.byte;
}

uint8_t si4735_configure_rds(FM_RDS_INT_SOURCE_STC rds_int, FM_RDS_CONFIG_STC rds_config, FM_RDS_INT_FIFO_COUNT_STC rds_fifo)
{
	si4735_set_property(FM_RDS_INT_SOURCE, rds_int.byte);

	si4735_set_property(FM_RDS_INT_FIFO_COUNT, rds_fifo.byte);

	si4735_set_property(FM_RDS_CONFIG, rds_config.byte);

	return 0;
}

uint8_t si4735_configure_seeking(uint16_t top_freq, uint16_t bot_freq, uint8_t freq_spacing, uint8_t snr, uint8_t rssi)
{
	//TODO setup bottom and top frequency for seeking
	si4735_set_property(FM_SEEK_BAND_BOTTOM, top_freq);

	si4735_set_property(FM_SEEK_BAND_TOP, bot_freq);

	//TODO implement a control on value for freqency space, snr and rssi only allowed values
	si4735_set_property(FM_SEEK_FREQ_SPACING, freq_spacing);

	si4735_set_property(FM_SEEK_TUNE_SNR_THRES, snr);

	si4735_set_property(FM_SEEK_TUNE_RSSI_THRES, rssi);
	return 0;
}

uint8_t si4735_wait_for_cts()
{
	//TODO implement a function which wait so long until CTS was send
	return 0;
}

uint8_t si4735_wait_for_command_completed()
{
	//TODO Set timer to 10 ms only after this can be send next command
	return 0;
}

void SI4735_INIT(void)	// Enthält alle für den Start benötigten Parameter
{
   //SEN
	ext_interrupt_create(SI_INT_INT, si4735_interrupt);

	SI_RST_DIR |= SI_RST_PIN;
	SI_EN_DIR |= SI_EN_PIN;

	SI_RST_OUT &= ~SI_RST_PIN;
	_delay_us(10);
	SI_RST_OUT |= SI_RST_PIN;
	_delay_us(10);

	SI_EN_OUT &= ~SI_EN_PIN;
	//P3DIR |=   BIT4;
	//P3OUT &=~  BIT4;

	//P2DIR |= BIT4;

	// Clear Reset
	//P2DIR |=  BIT3;
	//P2OUT &=~  BIT3;
	// Set Reset

	GPO_IEN_STC gpo_ien = {0};
	gpo_ien.STCIEN = 1;
	gpo_ien.CTSIEN = 1;

	//P2OUT |=  BIT3;		// Ausgang auf low gesetzt

	_delay_ms(1);

	//GPO2/INT
	SI_INT_REN |= SI_INT_PIN;
	SI_INT_OUT |= SI_INT_PIN;
	//ext_interrupt_enable(SI_INT_INT);
	//SI_INT_IES |= SI_INT_PIN;
	//SI_INT_IE |= SI_INT_PIN;
	//P2REN |= BIT4;
	//P2OUT |= BIT4;

	_delay_ms(1);

	ext_interrupt_enable(SI_INT_INT);

	/* Power_up FM mode */
	i2c_write_var(I2C_SI4735, STOP, 3,  0x01, 0xD0, 0x05);
	si4735_get_interrupt(8);

	/*activate and setting refclk to 32.768 kHz */
	si4735_set_property(REFCLK_FREQ, 0x8000);

	/* setup prescaler to 1 */
	si4735_set_property(REFCLK_PRESCALE, 1);

	/* setting deemphasis to 50µs standart for europe */
	si4735_set_property(FM_DEEMPH, DEEMPH_EU);


	const FM_RDS_INT_SOURCE_STC rds_int = {
			.RDSRECV = 1,
			.RDSSYNCLOST = 0,
			.RDSSYNCFOUND = 0,
			.RDSNEWBLOCKA = 0,
			.RDSNEWBLOCKB = 0
	};

	const FM_RDS_INT_FIFO_COUNT_STC rds_fifo = {
			.RDSFIFOCNT = 10,
	};

	const FM_RDS_CONFIG_STC rds_config = {
			.BLETHA = BLETH_1_2_ERR,
			.BLETHB = BLETH_1_2_ERR,
			.BLETHC = BLETH_1_2_ERR,
			.BLETHD = BLETH_1_2_ERR,
			.RDSEN = 1
	};

	si4735_configure_rds(rds_int, rds_config, rds_fifo);

	si4735_set_property(GPO_IEN, gpo_ien.byte);
	//_delay_ms(10);

	/* Tune to frequency 107,7 */
	si4735_fm_tune_freq(RADIO_BOT_FREQ);

	si4735_configure_seeking(RADIO_BOT_FREQ, RADIO_TOP_FREQ, RADIO_SEEK_FREQ_SPACE, RADIO_VALID_SNR, RADIO_VALID_RSSI);

	_delay_ms(10);

	si4735_fm_seek_start(1);

	_delay_ms(2);

	/* Enable radio audio output with standart value */
	si4735_set_property(RX_VOLUME, SI4735_volume);
	//i2c_write_var(I2C_SI4735, STOP, 6, 0x12, 0x00, 0x40, 0x00, 0x00, SI4735_volume);

	//uint8_t resp[8];



	//si4735_fm_tune_status(0, 0, resp);

	//si4735_get_interrupt(8);

	ext_interrupt_disable(SI_INT_INT);
	//while((SI_INT_IN & SI_INT_PIN));
}

void si4735_get_interrupt(uint8_t int_number)
{
	INT_STATUS status;
	while(!si_interrupt);
	si_interrupt = 0;
	if(int_number < 8)
	while(!si_interrupt);
	si_interrupt = 0;
	//_delay_us(10);
	/*if(int_number < 8) {
		//_delay_ms(10);
		do {
			i2c_write_var(I2C_SI4735, REPT, 1, 0x14);
			i2c_read(I2C_SI4735, STOP, 1, &status.byte);
		}
		while(status.byte == 0);
	}*/
	//return 1;
}

void si4735_interrupt(void)
{
	si_interrupt = 1;
}
