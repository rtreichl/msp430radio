/*
 * Encoder.h
 *
 *  Created on: 13.12.2013
 *      Author: Richi
 */

#ifndef ENCODER_H_
#define ENCODER_H_

#include <stdint.h>
#include <msp430.h>
#include <settings\radio_pin_mapping.h>
#include <settings\radio_configuration.h>
#include <driver/external_interrupthandler.h>

void Encoder_Timer_init(void);
#define TIME_FOR_LONG_PRESS 150

#define ENCODER_1	//Enable Encoder 1
#define ENCODER_2	//Enable Encoder 2

void encoder_interrupt(void);
void encoder_interrupt2(void);

#ifdef ENCODER_1 //If Encoder 1 is selected

#define ENCODER_1_ROTARY 0			//Rotary of Encoder 1 := 0 for right and 1 for left
#define ENCODER_1_PIN	EN1_A_IN 	//Port Input from Encoder 1
#define ENCODER_1_DIR	EN1_A_DIR 	//Direction from Encoder 1
#define ENCODER_1_IFG	EN1_A_IFG	//Interrupt Flag from Encoder 1
#define ENCODER_1_IES	EN1_A_IES	//Interrupt Edge Selector from Encoder 1
#define ENCODER_1_REN	EN1_A_REN	//Pullup/Pulldown from Encoder 1
#define ENCODER_1_OUT	EN1_A_OUT	//Port Output from Encoder 1
#define ENCODER_1_IE	EN1_A_IE	//Interrupt Enable from Encoder 1
#define ENCODER_1_A		EN1_A_PIN	//PIN A from Encoder 1
#define ENCODER_1_B		EN1_B_PIN	//PIN B from Encoder 1
#define ENCODER_1_T		EN1_TAST_PIN//PIN Button from Encoder 1

//Functions from Encoder 1
void Encoder_1_init(void); 		//Initiation from Encoder 1
void Encoder_1_decoder(void);	//Decoder from Encoder 1
int16_t encoder_1_update(int8_t *count, uint8_t **button);

#endif // ENCODER_1

#ifdef ENCODER_2	//If Encoder 2 is selected

#define ENCODER_1_ROTARY 0		//Rotary of Encoder 2 := 0 for right and 1 for left
#define ENCODER_2_PIN	P2IN	//PIN from Encoder 2
#define ENCODER_2_DIR	P2DIR	//Direction from Encoder 2
#define ENCODER_2_IFG	P2IFG	//Interrupt Flag from Encoder 2
#define ENCODER_2_IES	P2IES	//Interrupt Edge Selector from Encoder 2
#define ENCODER_2_REN	P2REN	//Pullup/Pulldown from Encoder 2
#define ENCODER_2_OUT	P2OUT	//Port Output from Encoder 2
#define ENCODER_2_IE	P2IE	//Interrupt Enable from Encoder 2
#define ENCODER_2_A		BIT0	//PIN A from Encoder 2
#define ENCODER_2_B		BIT1	//PIN B from Encoder 2
#define ENCODER_2_T		BIT2	//PIN Button from Encoder 2


//Functions from Encoder 2
void Encoder_2_init(void); 		//Initiation from Encoder 2
void Encoder_2_decoder(void);	//Decoder from Encoder 2
int16_t encoder_2_update(int8_t *count, uint8_t **button);

#endif //ENCODER_2



#endif /* ENCODER_H_ */
