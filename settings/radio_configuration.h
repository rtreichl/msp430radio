#ifndef RADIO_CONF_H_
#define RADIO_CONF_H_

#include <si4735_cmd_prop.h>

/* Defines values for auto search and seek funktion */

#define RADIO_VALID_SNR			10		//10dB
#define RADIO_VALID_RSSI		20		//25dBµV
#define RADIO_BOT_FREQ			0x222E 	//87,50 MHz
#define RADIO_TOP_FREQ			0x2A30	//108,00 MHz
#define RADIO_SEEK_FREQ_SPACE	SKSPACE_10 //100 kHz


#endif /* RADIO_CONF_H_ */
