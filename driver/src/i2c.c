/*
 * USCIT_I2C.c
 *
 *  Created on: 03.07.2013
 *      Author: Alexander Raskopf
 *      Status: Nach Test
 *
 *   Kommentar: Die einzelnen Funktionen für Initialisieren, Schreiben und Lesen, wurden den TI Beispielen
 *   			entnohmen und agepasst.
 *   			Master TX Multiple Byte nach D. Dang
 *   			Master RX Multiple Byte nach D. Dang
 */


#include <msp430.h>
#include <msp430g2553.h>
#include <intrinsics.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdarg.h>
#include "MSP430G2553_USCI_I2C.h"
#include "Timer.h"
//#include "I2C.h"
uint8_t stop = 1;

unsigned char *PTxData = 0 ;                     // Pointer to TX data
unsigned char *PTxData2 = 0 ;                     // Pointer to TX data
uint8_t act_size = 0;
//char *CMD;                                   // Pointer to TX data
unsigned char TXByteCtr;
unsigned char *PRxData;                      // Pointer to RX data
unsigned char RXByteCtr;
//unsigned char RxBuffer[128];                 // Allocate 128 byte of RAM



void USCI_I2C_INIT (uint8_t Teiler) // Initialisiert USCI_B0 für I2C Kommunikation
																	  // An die Funktion wird der Teil für die Frequenz und die Slave Adresse übergeben
{
	UCB0CTL1 |= UCSWRST;                      // Enable SW reset
	P1SEL |= BIT6 + BIT7;                     // Assign I2C pins to USCI_B0
	P1SEL2|= BIT6 + BIT7;                     // Assign I2C pins to USCI_B0

	UCB0CTL0 = UCMST + UCMODE_3 + UCSYNC;     // I2C Master, synchronous mode
	UCB0CTL1 = UCSSEL_2 + UCSWRST;            // Use SMCLK, keep SW reset
	UCB0BR0 = Teiler;                         // fSCL = SMCLK/12 = ~100kHz
	UCB0BR1 = 0;								//
	UCB0CTL1 &= ~UCSWRST;                     // Clear SW reset, resume operation*/
}

void USCI_I2C_WRITE2 (uint8_t addr, uint8_t rept_start, uint8_t n_args, ...)
{
	/* Reserve size of memory for transmit buffer */
	if(PTxData2 == 0) {
		PTxData2 = (uint8_t *) malloc(n_args * sizeof(uint8_t));
		act_size = n_args;
	}
	else {
		if(act_size != n_args) {
			PTxData2 = (uint8_t *) realloc(PTxData2, n_args * sizeof(uint8_t));
			act_size = n_args;
		}
	}

	/* Set slave address */
	UCB0I2CSA = addr;

	uint8_t i = 0;
	/* Read n_args from function
	 * Copy args to transmit buffer */
	va_list ap;
	va_start(ap, n_args);
	for(i = 0; i < n_args; i++)
		PTxData2[i] = va_arg(ap, uint8_t);
	va_end(ap);

	/* Clear Interrupt */
	IE2 = 0;

	/* Load TX byte counter */
	TXByteCtr = n_args;

	/* Wait unitl stop condition was send */
	while (UCB0CTL1 & UCTXSTP);

	/* Send a start condition with write flag */
	UCB0CTL1 |= UCTR + UCTXSTT;
	i = 0;
	/* Wait until start condtion is send */
	while (!(IFG2 & UCB0TXIFG));
	do {
		/* Copy value from buffer to transmit register */
		UCB0TXBUF = PTxData2[i++];

		/* Clear transmition flag to go forward on sendig */
		IFG2 &= ~UCB0TXIFG;

		/* Wait until data are send */
		while (!(IFG2 & UCB0TXIFG));
	} while(--TXByteCtr);

	if(rept_start == STOP) {
		/* Sending Stop Conditon */
		UCB0CTL1 |= UCTXSTP;

		/* Wait until stop condtion was send */
		while (UCB0CTL1 & UCTXSTP);
	}
	IFG2 &= ~UCB0TXIFG;
}

void USCI_I2C_READ (uint8_t addr, uint8_t rept_start, uint8_t RXBytes, uint8_t *RxData) // Liest eine Anzahl an Bytes vom Slave und übergibt diese an den Master
{
	/* Change pointer to given buffer */
	PRxData = (unsigned char *)RxData;
	/* Load RX counter */
	RXByteCtr = RXBytes;

	/* Clear Interrupt */
	IE2 = 0;

	/* Wait unitl stop condition was send */
	while (UCB0CTL1 & UCTXSTP);

	/* configure receiver mode */
	UCB0CTL1 &= ~UCTR;

	/* Send a start condition with read flag */
	UCB0CTL1 |= UCTXSTT;
	while(UCB0CTL1 & UCTXSTT);
	if ( RXByteCtr == 1 ) {
		UCB0CTL1 |= UCTXSTP;
		RXByteCtr = 0;
	}
	else {
		RXByteCtr -= 2;
	}
	do {
		/* write received data in buffer */

		if(RXByteCtr == 0) {
			UCB0CTL1 |= UCTXSTP;
		}
		while (!(IFG2 & UCB0RXIFG));
		*PRxData++ = UCB0RXBUF;
		IFG2 &= ~UCB0RXIFG;

		/* clear receive interrupt flag */

		/* Wait until transmition interrupt flag is set */
		//while (!(IFG2 & UCB0RXIFG));
	}
	while(RXByteCtr--);

	if(stop != 0) {
		//UCB0CTL1 |= UCTXSTP;

		/* Wait until stop condtion was send */
		while (UCB0CTL1 & UCTXSTP);
	}
}
