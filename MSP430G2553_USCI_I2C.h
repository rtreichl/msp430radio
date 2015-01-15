/*
 * USCIT_I2C.h
 *
 *  Created on: 03.07.2013
 *      Author: Alexander Raskopf
 */

#ifndef USCIT_I2C_H_
#define USCIT_I2C_H_

void USCI_I2C_INIT (unsigned char SlaveAdresse, unsigned char Teiler);

void USCI_I2C_WRITE1 (unsigned char Byts, char *CMD);

//void USCI_I2C_WRITE2 (unsigned char Byts, unsigned char *CMD);

void USCI_I2C_READ (unsigned char RXBytes);
void I2C_write_and_read(unsigned char TxByts,  char *TxData, unsigned char RxByts,  char *RxData);


#endif /* USCIT_I2C_H_ */
