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
#include <driver/include/si4735_cmd_prop.h>
#include <settings/radio_configuration.h>
#include <driver/external_interrupthandler.h>

volatile uint8_t si_interrupt;

void si4735_get_interrupt(uint8_t int_number);

#define SI4735_volume 5 		// Maximalwert für die Lautstärke
#define SI4735_volume_start  20 // Startlautstärke

uint8_t si4735_power_up(uint8_t mode)	//einschalten des Si4735 aktivieren des Analogen Audio Ausgangs
{
	i2c_write_var(I2C_SI4735, STOP, 3,  0x01, 0xD0, 0x05);
	si4735_get_interrupt(8);
	return 0;
}

uint8_t si4735_power_down(void)	//einschalten des Si4735 aktivieren des Analogen Audio Ausgangs
{
	i2c_write_var(I2C_SI4735, STOP, 1, 0x11);

	SI_RST_OUT &= ~SI_RST_PIN;
	SI_EN_OUT |= SI_EN_PIN;


	return 0;
}

void SI4735_Rx_Hard_Mute (void)	//setzt den Audio Ausgang auf stumm, L und R Audio Ausgang könnten selbständig auf stumm geschaltet sein
{
	i2c_write_var(I2C_SI4735, STOP, 6, 0x12, 0x00, 0x40, 0x01, 0x00, 0x00);
}

uint8_t si4735_fm_rds_status(uint8_t statusonly, uint8_t mtfifo, uint8_t intack)
{
	uint8_t resp[13];
	SI4735_FM_RDS_STATUS_ARG arg = {.INTACK = intack , .MTFIFO = mtfifo, .STATUSONLY = statusonly};

	i2c_write_var(I2C_SI4735, REPT, 2, SI4735_FM_RDS_STATUS, arg);
	i2c_read(I2C_SI4735, STOP, I2C_LITTLE_ENDIAN, 13, resp);

	return resp[0];
}

uint8_t si4735_fm_rsq_status(uint8_t intack, uint8_t *resp)
{
	SI4735_FM_RSQ_STATUS_ARG arg = { .INTACK = intack};

	i2c_write_var(I2C_SI4735, REPT, 2, SI4735_FM_RSQ_STATUS, arg);
	i2c_read(I2C_SI4735, STOP, I2C_LITTLE_ENDIAN, 8, resp);

	return resp[0];
}

uint8_t si4735_fm_tune_status(uint8_t cancel, uint8_t intack, uint8_t *resp)
{
	SI4735_FM_TUNE_STATUS_ARG arg = { .CANCEL = cancel, .INTACK = intack};

	i2c_write_var(I2C_SI4735, REPT, 2, SI4735_FM_TUNE_STATUS, arg);
	_delay_ms(10);
	//si4735_get_interrupt(8);
	i2c_read(I2C_SI4735, STOP, I2C_LITTLE_ENDIAN, 8, resp);

	return resp[0];
}

SI4735_FM_STATUS si4735_fm_tune_freq(uint16_t frequency)
{
	SI4735_FM_STATUS status;
	const FM_TUNE_FREQ_ARG1_STC arg1 = { .FREEZE = 0, .FAST = 0};

	i2c_write_var(I2C_SI4735, STOP, 5, FM_TUNE_FREQ, arg1.byte, HB(frequency), LB(frequency), 0x00);
	si4735_get_interrupt(7);
	i2c_read(I2C_SI4735, STOP, I2C_LITTLE_ENDIAN, 1, &status);

	return status;
}

SI4735_FM_STATUS si4735_get_int_status()
{
	SI4735_FM_STATUS status;
	//TODO check int pin from SI4735 via interrupt routin if interrupt availible read.
	i2c_write_var(I2C_SI4735, REPT, 1, GET_INT_STATUS);
	i2c_read(I2C_SI4735, STOP, I2C_LITTLE_ENDIAN, 1, &status);

	return status;
}

uint16_t si4735_get_property(uint16_t property, uint16_t *data)
{
	uint8_t resp[4];

	i2c_write_var(I2C_SI4735, STOP, 6, SET_PROPERTY, 0x00, HB(property), LB(property));
	i2c_read(I2C_SI4735, STOP, I2C_LITTLE_ENDIAN, 4, resp);

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

SI4735_FM_STATUS si4735_fm_seek_start(uint8_t up_down)	//Frequensuchlauf für höhere Frequenzen
{
	SI4735_FM_STATUS status;
	FM_SEEK_START_ARG1_STC arg1 = {.SEEKUP = 0, .WRAP = 1};

	arg1.SEEKUP = up_down;
	i2c_write_var(I2C_SI4735, STOP, 2, FM_SEEK_START, arg1.byte);
	si4735_get_interrupt(7);
	i2c_read(I2C_SI4735, STOP, I2C_LITTLE_ENDIAN, 1, &status);

	return status;
}

uint8_t si4735_configure_rds(const FM_RDS_INT_SOURCE_STC *rds_int, const FM_RDS_CONFIG_STC *rds_config, const FM_RDS_INT_FIFO_COUNT_STC *rds_fifo)
{
	si4735_set_property(FM_RDS_INT_SOURCE, rds_int->byte);

	si4735_set_property(FM_RDS_INT_FIFO_COUNT, rds_fifo->byte);

	si4735_set_property(FM_RDS_CONFIG, rds_config->byte);

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

uint8_t si4735_init(uint8_t volume, uint16_t freq)	// Enthält alle für den Start benötigten Parameter
{
   //SEN
	ext_interrupt_create(SI_INT_INT, si4735_interrupt);

	SI_RST_DIR |= SI_RST_PIN;
	SI_EN_DIR |= SI_EN_PIN;

	SI_RST_OUT &= ~SI_RST_PIN;
	_delay_ten_us(10);
	SI_RST_OUT |= SI_RST_PIN;
	_delay_ten_us(10);

	SI_EN_OUT &= ~SI_EN_PIN;

	// Clear Reset

	// Set Reset

	const GPO_IEN_STC gpo_ien = {
			.STCIEN = 1,
			.CTSIEN = 1,
			.RDSIEN = 0,
			.RSQIEN = 0,
			.ERRIEN = 0,
			.STCREP = 1
	};

	_delay_ms(1);

	//GPO2/INT
	SI_INT_REN |= SI_INT_PIN;
	SI_INT_OUT |= SI_INT_PIN;

	_delay_ms(1);

	ext_interrupt_enable(SI_INT_INT);

	/* Power_up FM mode */
	si4735_power_up(0);

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

	si4735_configure_rds(&rds_int, &rds_config, &rds_fifo);

	si4735_set_property(GPO_IEN, gpo_ien.byte);
	//_delay_ms(10);

	/* Tune to frequency 107,7 */
	si4735_fm_tune_freq(freq);

	si4735_configure_seeking(RADIO_BOT_FREQ, RADIO_TOP_FREQ, RADIO_SEEK_FREQ_SPACE, RADIO_VALID_SNR, RADIO_VALID_RSSI);
	_delay_ms(10);
	//si4735_fm_seek_start(1);
	//_delay_ms(2);

	/* Enable radio audio output with standart value */
	si4735_set_property(RX_VOLUME, SI4735_volume);

	/* Disable interrupt for si4735 */
	ext_interrupt_disable(SI_INT_INT);

	return 0;
}

void si4735_get_interrupt(uint8_t int_number)
{
	while(!si_interrupt) {
	}
	si_interrupt = 0;
	if(int_number < 8) {
		while(!si_interrupt) {
		}
	}
	si_interrupt = 0;
}

void si4735_interrupt(void)
{
	si_interrupt = 1;
}
