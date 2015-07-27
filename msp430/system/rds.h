/*
 * Radio.h
 *
 *  Created on: 21.01.2014
 *      Author: Richi
 */


//----------------------------------------------------------------------------------------
//
/// \brief In here the are structs declared for several RDS stuff(See http://www.g.laroche.
///	free.fr/english/rds/groupes/listeGroupesRDS.htm for more information). Also there are
///	functions declared to update RDS and to get the	current time out of RDS.
//
//----------------------------------------------------------------------------------------

#ifndef RADIO_H_
#define RADIO_H_

#include <stdint.h>
#include <driver/i2c.h>
#include <driver/si4735.h>
#include <system/time.h>
#include <driver/timer.h>
#include <radio/radio.h>

#define RDS_GROUP_NUM_0	0
#define RDS_GROUP_NUM_1	1
#define RDS_GROUP_NUM_2	2
#define RDS_GROUP_NUM_4	4

#define RDS_GROUP_VERSION_A 0
#define RDS_GROUP_VERSION_B	1

#define RDS_RADIO_TEXT_MAX_SYMBOLS			64
#define RDS_RADIO_TEXT_SYMBOLS_PER_FRAME	4
#define RDS_STATION_NAME_MAX_SYMBOLS		8
#define RDS_STATION_NAME_SYMBOLS_PER_FRAME	2

#define RDS_FIFO_MAX_SIZE					10

#define RDS_BLOCK_ERROR_UNCORRECTABLE		3
#define RDS_INTERRUPT						0x04

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
	uint8_t PS_NAME[2];
	uint8_t AF[2];
	uint16_t CI : 2;
	uint16_t DI : 1;
	uint16_t MS : 1;
	uint16_t TA : 1;
	uint16_t PTY : 5;
	uint16_t TP : 1;
	uint16_t GROUP_CHAR : 1;
	uint16_t GROUP_NUM : 4;
} GROUP_0A;

typedef struct group_0b {
	uint8_t PS_NAME[2];
	uint8_t AF[2];
	uint16_t CI : 2;
	uint16_t DI : 1;
	uint16_t MS : 1;
	uint16_t TA : 1;
	uint16_t PTY : 5;
	uint16_t TP : 1;
	uint16_t GROUP_CHAR : 1;
	uint16_t GROUP_NUM : 4;
} GROUP_0B;

typedef struct group_2a {
	uint8_t SEGMENT[4];
	uint16_t B : 4;
	uint16_t A_B : 1;
	uint16_t PTY : 5;
	uint16_t TP : 1;
	uint16_t GROUP_CHAR : 1;
	uint16_t GROUP_NUM : 4;
} GROUP_2A;

typedef struct group_2b {
	uint8_t SEGMENT[2];
	uint16_t PI_CODE;
	uint16_t B : 4;
	uint16_t A_B : 1;
	uint16_t PTY : 5;
	uint16_t TP : 1;
	uint16_t GROUP_CHAR : 1;
	uint16_t GROUP_NUM : 4;
} GROUP_2B;

typedef struct group_1b {
	uint16_t PIN;
	uint16_t PI_CODE;
	uint16_t B : 5;
	uint16_t PTY : 5;
	uint16_t TP : 1;
	uint16_t GROUP_CHAR : 1;
	uint16_t GROUP_NUM : 4;

} GROUP_1B;

typedef struct group_1a {
	uint16_t PIN;
	uint16_t SLC;
	uint16_t B : 5;
	uint16_t PTY : 5;
	uint16_t TP : 1;
	uint16_t GROUP_CHAR : 1;
	uint16_t GROUP_NUM : 4;
} GROUP_1A;

typedef struct group_4a {
	int16_t TIME_OFF:6;
	uint16_t MINUTE:6;
	uint16_t HOUR_L:4;
	uint16_t HOUR_H:1;
	uint16_t DATE_L:15;
	uint16_t DATE_H:2;
	uint16_t:3;
	uint16_t PTY : 5;
	uint16_t TP : 1;
	uint16_t GROUP_CHAR : 1;
	uint16_t GROUP_NUM : 4;
} GROUP_4A;

typedef struct block_b {
	uint16_t blocks[2];
	uint16_t:5;
	uint16_t PTY:5;
	uint16_t TP:1;
	uint16_t GROUP_CHAR:1;
	uint16_t GROUP_NUM:4;
} BLOCK_B;

typedef struct rds {
	uint8_t algin;
	ERR		err;
	union {
		GROUP_0A group_0a;
		GROUP_0B group_0b;
		GROUP_1A group_1a;
		GROUP_1B group_1b;
		GROUP_2A group_2a;
		GROUP_2B group_2b;
		GROUP_4A group_4a;
		BLOCK_B	 block_b;
		uint8_t block[6];
	};
	uint16_t pi;
	FIFO	fifo;
	SYNC	sync;
	INFO	info;
	STATUS 	status;
} RDS;

void rds_update(RADIO *radio);
uint8_t rds_triggered();

void rds_group_4A(RDS *data);

#endif /* RADIO_H_ */
