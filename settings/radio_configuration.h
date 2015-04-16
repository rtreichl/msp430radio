#ifndef RADIO_CONF_H_
#define RADIO_CONF_H_

#include <driver/include/si4735_cmd_prop.h>

/* Defines values for auto search and seek funktion */

#define RADIO_VALID_SNR			15				//10dB
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

/* Define macros for factory state */

#define RADIO_FACORY_FREQENCY	0				//Seekup on start
#define RADIO_FACORY_VIEW		0				//RDS view
#define RADIO_FACORY_VOLUME		20				//Normal volume 20%
#define RADIO_FACORY_EQUALIZER	0				//Equalizer POP
#define RADIO_FACORY_SOURCE		0				//Source FM
#define RADIO_FACORY_BRIGHTNESS	35				//Brightness 70%
#define RADIO_FACORY_CONTRAST	6				//Contrast 60%
#define RADIO_FACORY_TATP		0				//TA TP off
/* Define macros for time, date for cold start */

#define RADIO_DATE_DAY			BUILD_DAY		//Day where firmware is build
#define RADIO_DATE_MONTH		BUILD_MONTH		//Month where firmware is build
#define RADIO_DATE_YEAR			BUILD_YEAR		//Year where firmware is build
#define RADIO_TIME_HOUR			BUILD_HOUR		//Hour where firmware is build
#define RADIO_TIME_MINUTE		BUILD_MIN		//Minute where firmware is build


#endif /* RADIO_CONF_H_ */
