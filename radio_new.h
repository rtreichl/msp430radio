/*
 * radio_new.h
 *
 *  Created on: 02.02.2015
 *      Author: Richard
 */

#ifndef RADIO_NEW_H_
#define RADIO_NEW_H_

#define MSP_RXD			1, 1
#define MSP_TXD			1.2
#define EN1_A			1.3
#define EN1_B			1.4
#define EN1_TAST		1.5
#define SI_RST			3.1
#define SI_INT			3.0
#define SI_EN			3.2
#define EN2_A			2.0
#define EN2_B			2.1
#define EN2_TAST		2.2
#define AUIDO_SW_GND	3.2
#define AUDIO_SW_LINE	3.3
#define PCA_ON_OFF		2.4
#define PCA_INT			2.5
#define ANT_MF			3.5
#define ANT_LF			3.6
#define AMP_SHUTDOWN	3.7
#define SCL				1.6
#define SDA				1.7
#define PWM_RST			1.0

#define DEFINE_SEL(a, b) P##a##SEL
#define MSP_RXD_SEL DEFINE_SEL(MSP_RXD)

int x = MSP_RXD_SEL;

#endif /* RADIO_NEW_H_ */
