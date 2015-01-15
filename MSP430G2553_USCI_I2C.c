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
#include "MSP430G2553_USCI_I2C.h"
#include "Timer.h"
//#include "I2C.h"


unsigned char *PTxData ;                     // Pointer to TX data
//char *CMD;                                   // Pointer to TX data
unsigned char TXByteCtr;
unsigned char *PRxData;                      // Pointer to RX data
unsigned char RXByteCtr;
//unsigned char RxBuffer[128];                 // Allocate 128 byte of RAM



void USCI_I2C_INIT (unsigned char SlaveAdresse, unsigned char Teiler) // Initialisiert USCI_B0 für I2C Kommunikation
																	  // An die Funktion wird der Teil für die Frequenz und die Slave Adresse übergeben
{
	//I2C_init(SlaveAdresse, Teiler);

  P1SEL |= BIT6 + BIT7;                     // Assign I2C pins to USCI_B0
  P1SEL2|= BIT6 + BIT7;                     // Assign I2C pins to USCI_B0
  UCB0CTL1 |= UCSWRST;                      // Enable SW reset
  UCB0CTL0 = UCMST + UCMODE_3 + UCSYNC;     // I2C Master, synchronous mode
  UCB0CTL1 = UCSSEL_2 + UCSWRST;            // Use SMCLK, keep SW reset
  UCB0BR0 = Teiler;                         // fSCL = SMCLK/12 = ~100kHz
  UCB0BR1 = 0;								//
  UCB0I2CSA = SlaveAdresse;                 // Slave Address ist SlaveAdresse variabel
  UCB0CTL1 &= ~UCSWRST;                     // Clear SW reset, resume operation*/
}


void USCI_I2C_WRITE1 (unsigned char Byts, char *CMD) // Liest eine Anzahl an Bytes in ein Array ein und Schreib diese an den Slave
{
	//I2C_write(Byts,CMD,0);


	IE2 = 0;
	PTxData = (unsigned char*)CMD;    // TX array start address
	//PRxData = (unsigned char*)RxData;                      // TX array start address
	//RXByteCtr = RxByts;                  // Load TX byte counter
	TXByteCtr = Byts;                // Load TX byte counter
	while (UCB0CTL1 & UCTXSTP);
	UCB0CTL1 |= UCTR + UCTXSTT;
	do
	{
		while (!(IFG2 & UCB0TXIFG));
		_delay_us(10);
		UCB0TXBUF = *PTxData++;

		IFG2 &= ~UCB0TXIFG;
	}
	while(--TXByteCtr);
	while (!(IFG2 & UCB0TXIFG));
	IFG2 &= ~UCB0TXIFG;
	//_delay_us(5);
	//while (!(IFG2 & UCB0TXIFG));
	UCB0CTL1 |= UCTXSTP;
	while (UCB0CTL1 & UCTXSTP);
	/*
	__bis_SR_register(GIE);
	IE2 |= UCB0TXIE;
    PTxData = (unsigned char *)CMD;         // TX array start address
    TXByteCtr =Byts;
                // Ensure stop condition got sent
    UCB0CTL1 |= UCTR + UCTXSTT;             // I2C TX, start condition
    __bis_SR_register(CPUOFF + GIE);        // Enter LPM0 w/ interrupts
                                            // Remain in LPM0 until all data

                                           // is TX'd*/
}







void USCI_I2C_READ (unsigned char RXBytes) // Liest eine Anzahl an Bytes vom Slave und übergibt diese an den Master
{
//	unsigned char CMD1[10];
//	I2C_read(RXBytes,CMD1,0);

		IE2 |= UCB0RXIE;

	    //PRxData = (unsigned char *)RxBuffer;    // Start of RX buffer
	    RXByteCtr = RXBytes;                          // Load RX byte counter
	    while (UCB0CTL1 & UCTXSTP);             // Ensure stop condition got sent
	    UCB0CTL1 &=~ UCTR;
	    UCB0CTL1 |= UCTXSTT;                    // I2C start condition
	    __bis_SR_register(CPUOFF + GIE);        // Enter LPM0 w/ interrupts
	                                            // Remain in LPM0 until all data
	                                            // is RX'd
	    __no_operation();                       // Set breakpoint >>here<< and
	    										// read out the RxBuffer buffer

}

void I2C_write_and_read(unsigned char TxByts,  char *TxData, unsigned char RxByts,  char *RxData)
{
	while (UCB0CTL1 & UCTXSTP);
	IE2 = 0;
	PTxData = (unsigned char*)TxData;    // TX array start address
	PRxData = (unsigned char*)RxData;                      // TX array start address
	RXByteCtr = RxByts;                  // Load TX byte counter
	TXByteCtr = TxByts;                // Load TX byte counter
	UCB0CTL1 |= UCTR + UCTXSTT;
	do
	{
		while (!(IFG2 & UCB0TXIFG));
		UCB0TXBUF = *PTxData++;
		IFG2 &= ~UCB0TXIFG;
	}
	while(--TXByteCtr);
	while (!(IFG2 & UCB0TXIFG));
	IFG2 &= ~UCB0TXIFG;
	//while (UCB0CTL1 & UCTXSTP);
	UCB0CTL1 &=~ UCTR;
	_delay_us(100);
	UCB0CTL1 |= UCTXSTT;
	do
	{
			while (!((IFG2 & UCB0RXIFG) && (UCB0STAT & UCSCLLOW)));
			*PRxData++ = UCB0RXBUF;
			IFG2 &= ~UCB0RXIFG;
	}
	while(--RXByteCtr);
	//while (!(IFG2 & UCB0RXIFG));
	UCB0CTL1 |= UCTXSTP;
	while (UCB0CTL1 & UCTXSTP);
}

/*
#pragma vector = USCIAB0TX_VECTOR		//Steuert das Senden und Empfangen von Bytes
__interrupt void USCIAB0TX_ISR(void)
{
if(IFG2 & UCB0TXIFG)
{
  if (TXByteCtr)                            // Check TX byte counter
  {
    UCB0TXBUF = *PTxData++;                 // Load TX buffer
    TXByteCtr--;                            // Decrement TX byte counter
  }
  else
  {
    UCB0CTL1 |= UCTXSTP;                    // I2C stop condition
    IFG2 &= ~UCB0TXIFG;                     // Clear USCI_B0 TX int flag
    __bic_SR_register_on_exit(CPUOFF);      // Exit LPM0
  }
}
else
if(IFG2 & UCB0RXIFG)
{

  RXByteCtr--;                              // Decrement RX byte counter
  if (RXByteCtr)
  {
    *PRxData++ = UCB0RXBUF;                 // Move RX data to address PRxData
    if (RXByteCtr == 1)                     // Only one byte left?
    UCB0CTL1 |= UCTXSTP;                    // Generate I2C stop condition
  }
  else
  {
    *PRxData = UCB0RXBUF;                   // Move final RX data to PRxData
    __bic_SR_register_on_exit(CPUOFF);      // Exit LPM0
  }
}
}
*/
