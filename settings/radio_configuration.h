#ifndef RADIO_CONF_H_
#define RADIO_CONF_H_

#include <driver/include/si4735_cmd_prop.h>

/* Defines values for auto search and seek funktion */

#define RADIO_VALID_SNR			10				//10dB
#define RADIO_VALID_RSSI		20				//25dBµV
#define RADIO_BOT_FREQ			0x222E 			//87,50 MHz
#define RADIO_TOP_FREQ			0x2A30			//108,00 MHz
#define RADIO_SEEK_FREQ_SPACE	SKSPACE_10		//100 kHz
#define RADIO_CALBC1			CALBC1_16MHZ	//16 MHz
#define RADIO_CALDCO			CALDCO_16MHZ	//16 MHz
#define RADIO_DCO_FREQ			16				//16 MHz
#define RADIO_LCD_REFRESCH		500				//500 ms
#define RADIO_ENCODER_POLL		100				//100 ms
#define RADIO_MCLK				16				//16 MHz
#define RADIO_SCLK				2				//2 MHz


#endif /* RADIO_CONF_H_ */
