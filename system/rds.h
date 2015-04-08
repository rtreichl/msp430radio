/*
 * Radio.h
 *
 *  Created on: 21.01.2014
 *      Author: Richi
 */

#ifndef RADIO_H_
#define RADIO_H_

#include <stdint.h>
#include <driver/i2c.h>
#include <driver/si4735.h>
#include <system/time.h>
#include <driver/timer.h>
#include <system/radio.h>

//#define RDS_BYTES_OFFSET 2

typedef struct status {
	uint8_t STCINT:1;
	uint8_t:1;
	uint8_t RDSINT:1;
	uint8_t RSQINT:1;
	uint8_t:2;
	uint8_t ERR:1;
	uint8_t CTS:1;
} STATUS;

typedef struct info {
	uint8_t RDSRECV:1;
	uint8_t RDSSYNCLOST:1;
	uint8_t RDSSYNCFOUND:1;
	uint8_t:1;
	uint8_t RDSNEWBLOCKA:1;
	uint8_t RDSNEWBLOCKB:1;
	uint8_t:2;
} INFO;

typedef struct sync {
	uint8_t RDSSYNC:1;
	uint8_t:1;
	uint8_t GRPLOST:1;
	uint8_t:5;
} SYNC;

typedef struct fifo {
	uint8_t RDSFIFOUSED;
} FIFO;

typedef struct err {
	uint8_t BLED:2;
	uint8_t BLEC:2;
	uint8_t BLEB:2;
	uint8_t BLEA:2;
} ERR;

typedef struct group_0a {
	uint16_t PI_CODE;
	uint16_t CI : 2;
	uint16_t DI : 1;
	uint16_t MS : 1;
	uint16_t TA : 1;
	uint16_t PTY : 5;
	uint16_t TP : 1;
	uint16_t GROUP_CHAR : 1;
	uint16_t GROUP_NUM : 4;
	uint8_t AF[2];
	uint8_t PS_NAME[2];
} GROUP_0A;

typedef struct group_0b {
	uint16_t PI_CODE;
	uint16_t CI : 2;
	uint16_t DI : 1;
	uint16_t MS : 1;
	uint16_t TA : 1;
	uint16_t PTY : 5;
	uint16_t TP : 1;
	uint16_t GROUP_CHAR : 1;
	uint16_t GROUP_NUM : 4;
	uint8_t AF[2];
	uint8_t PS_NAME[2];
} GROUP_0B;

typedef struct group_2a {
	uint16_t PI_CODE;
	uint16_t B : 4;
	uint16_t A_B : 1;
	uint16_t PTY : 5;
	uint16_t TP : 1;
	uint16_t GROUP_CHAR : 1;
	uint16_t GROUP_NUM : 4;
	uint8_t SEGMENT[4];
} GROUP_2A;

typedef struct group_2b {
	uint16_t PI_CODE;
	uint16_t B : 4;
	uint16_t A_B : 1;
	uint16_t PTY : 5;
	uint16_t TP : 1;
	uint16_t GROUP_CHAR : 1;
	uint16_t GROUP_NUM : 4;
	uint16_t PI_CODE_2;
	uint8_t SEGMENT[2];
} GROUP_2B;

typedef struct group_1b {
	uint16_t PI_CODE;
	uint16_t B : 5;
	uint16_t PTY : 5;
	uint16_t TP : 1;
	uint16_t GROUP_CHAR : 1;
	uint16_t GROUP_NUM : 4;
	uint16_t PI_CODE_2;
	uint16_t PIN;
} GROUP_1B;

typedef struct group_1a {
	uint16_t PI_CODE;
	uint16_t B : 5;
	uint16_t PTY : 5;
	uint16_t TP : 1;
	uint16_t GROUP_CHAR : 1;
	uint16_t GROUP_NUM : 4;
	uint16_t SLC;
	uint16_t PIN;
} GROUP_1A;

typedef struct group_4a {
	uint16_t PI_CODE:16;
	uint16_t DATE_H:2;
	uint16_t:3;
	uint16_t PTY : 5;
	uint16_t TP : 1;
	uint16_t GROUP_CHAR : 1;
	uint16_t GROUP_NUM : 4;
	uint16_t HOUR_H:1;
	uint16_t DATE_L:15;
	int16_t TIME_OFF:6;
	uint16_t MINUTE:6;
	uint16_t HOUR_L:4;



} GROUP_4A;

typedef struct block_b {
	uint16_t:5;
	uint16_t PTY:5;
	uint16_t TP:1;
	uint16_t GROUP_CHAR:1;
	uint16_t GROUP_NUM:4;
} BLOCK_B;

typedef struct rds {
	STATUS 	status;
	INFO	info;
	SYNC	sync;
	FIFO	fifo;
	uint16_t	pi;
	BLOCK_B block_b;
	uint8_t block[4];
	ERR		err;
} RDS;

void rds_update(RADIO *radio);
uint8_t rds_triggered();
uint8_t get_signal_qual();

void rds_group_4A(RDS *data);

#endif /* RADIO_H_ */
