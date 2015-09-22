///////////////////////////////////////////////////////////////////////////////////////////
///
/// \file rds.h
///
/// \brief rds.h includes all necessary functions macros and structs for the radio data\n
/// service
///
/// \date 21.01.2014
///
/// \author Richard Treichl
///
/// \remark
///
/// \todo
///
/// \version	1.0
///
///////////////////////////////////////////////////////////////////////////////////////////

#ifndef RADIO_H_
#define RADIO_H_

#include <stdint.h>
#include <driver/i2c.h>
#include <driver/si4735.h>
#include <system/time.h>
#include <driver/timer.h>
#include <radio/radio.h>

///////////////////////////////////////////////////////////////////////////////////////////
///
/// \defgroup RDS Radio data Service Macros, Structs and Functions
/// \brief In here the are structs declared for several RDS stuff(See http://www.g.laroche.
///	free.fr/english/rds/groupes/listeGroupesRDS.htm for more information). Also there are
///	functions declared to update RDS and to get the	current time out of RDS.\n\n
///
/// The group RDS allows to identify the transmitted type of data. There are 16 groups\n
/// which can each decline in 2 versions A or B.\n
/// The group type and the version (A or B) are transmitted in the second block (block B)}n
/// of RDS group. The bits A0 -> A3 indicate the group type (0= group 0, .... F = group\n
/// 15) and the bit B0 indicate the version (0 = A version ).
///
///	@{
///
///////////////////////////////////////////////////////////////////////////////////////////

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

///////////////////////////////////////////////////////////////////////////////////////////
///
/// \struct group_0a
/// \brief RDS Group 0A
///
/// The 0A group allow to transmit basic data (TA, M/S, DI, C1/C0) as well as Alternatives\n
/// Frequency (AF) and the name of radio (PS Name)
///
/// \var group_0a::PS_NAME
/// \brief Program Service Name.
///
/// This 16 bits word allows to transmit 2 characters of radio name PS Name.The bit C1/C0\n
///	allows to select the transmitted characters.
///
/// \var group_0a::AF
/// \brief Alternative Frequency.
///
/// This word of 16bits allows to transmit alternative frequencies. These frequencies allow\n
/// the receiver to switch to another transmitter diffusing the same radio in case of bad\n
/// reception. This function is very useful for a radio car because it allow to know\n
/// quickly the other near transmitters and so to commute quickly when car moving. The \n
/// frequency is coded by step of 100KHz from 87.6MHz, a list of frequencies must be\n
/// always preceded by the consequentive number of the list (from 1 to 25 frequencies)
///
/// \var group_0a::CI
/// \brief Select of transmitted characters.
///
/// These 2 bits allow to select character of transmitted name (PS Name) as well as DI\n
/// information according to the following figure.
/// C1 | C0 | DI | PS1 | PS0
/// -- | -- | -- | --- | ---
///  0 |  0 | d3 |  1  |  2
///  0 |  1 | d2 |  3  |  4
///  1 |  0 | d1 |  5  |  6
///  1 |  1 | d0 |  7  |  8
///
/// \var group_0a::DI
/// \brief Decoder Information.
///
/// This bit allow to indicate to the receiver the type of transmission used, associated\n
/// to 2 bits C1/C0 it is possible to transmit 4 bits data.
///
/// \var group_0a::MS
/// \brief Music / speech.
///
/// If a musical emission is in course of transmission, the value "1" is set (default\n
/// value if unused function), otherwise the value "0" indicate a speech broadcasting.
///
/// \var group_0a::TA
/// \brief Traffic Annoucement.
///
/// set to "1" if traffic data are being diffused, otherwise this flag is set to "0".\n
/// (see also description flag TP)
///
/// \var group_0a::PTY
/// \brief Program Type.
///
/// The bits b5 to b9 indicate the type of program of the radio.
///
/// \var group_0a::TP
/// \brief Traffic Program.
///
/// The TP (Traffic Possible) flag indicate if program can carried Traffic information\n
/// according to the following:\n\n
///
/// 0 = This program does not carry traffic, on the other hand if TA is set, traffic\n
/// announcements can be broadcast on other program via EON\n
/// 1 = This program carries traffic announcements. If TA flag is set, a traffic\n
/// announcement is being broadcast on this program at present.
///
/// \var group_0a::GROUP_CHAR
/// \brief Group Version
///
///	0 = Group version A.\n
/// 1 = Group version B.\n
///
/// \var group_0a::GROUP_NUM
/// \brief Group Number.
///
/// Contains the number which identify the actual group data is transmitted
///
///////////////////////////////////////////////////////////////////////////////////////////

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

///////////////////////////////////////////////////////////////////////////////////////////
///
/// \struct group_0b
/// \brief RDS Group 0B
///
/// The 0B group allow to transmit basic data (TA, M/S, DI, C1/C0) and the name of radio\n
/// (PS Name)
///
/// \var group_0b::PS_NAME
/// \brief Program Service Name.
///
/// This 16 bits word allows to transmit 2 characters of radio name PS Name.The bit C1/C0\n
///	allows to select the transmitted characters.
///
/// \var group_0b::PI
/// \brief Program Identification Code.
///
/// The Program Identification Code (PI code) allows to identify radio with country, area\n
/// coverage and program reference number
///
/// \var group_0b::CI
/// \brief Select of transmitted characters.
///
/// These 2 bits allow to select character of transmitted name (PS Name) as well as DI\n
/// information according to the following figure.
/// C1 | C0 | DI | PS1 | PS0
/// -- | -- | -- | --- | ---
///  0 |  0 | d3 |  1  |  2
///  0 |  1 | d2 |  3  |  4
///  1 |  0 | d1 |  5  |  6
///  1 |  1 | d0 |  7  |  8
///
/// \var group_0b::DI
/// \brief Decoder Information.
///
/// This bit allow to indicate to the receiver the type of transmission used, associated\n
/// to 2 bits C1/C0 it is possible to transmit 4 bits data.
///
/// \var group_0b::MS
/// \brief Music / speech.
///
/// If a musical emission is in course of transmission, the value "1" is set (default\n
/// value if unused function), otherwise the value "0" indicate a speech broadcasting.
///
/// \var group_0b::TA
/// \brief Traffic Annoucement.
///
/// set to "1" if traffic data are being diffused, otherwise this flag is set to "0".\n
/// (see also description flag TP)
///
/// \var group_0b::PTY
/// \brief Program Type.
///
/// The bits b5 to b9 indicate the type of program of the radio.
///
/// \var group_0b::TP
/// \ref Traffic Program.
///
/// The TP (Traffic Possible) flag indicate if program can carried Traffic information\n
/// according to the following:\n\n
///
/// 0 = This program does not carry traffic, on the other hand if TA is set, traffic\n
/// announcements can be broadcast on other program via EON\n
/// 1 = This program carries traffic announcements. If TA flag is set, a traffic\n
/// announcement is being broadcast on this program at present.
///
/// \var group_0b::GROUP_CHAR
/// \brief Group Version
///
///	0 = Group version A.\n
/// 1 = Group version B.\n
///
/// \var group_0b::GROUP_NUM
/// \brief Group Number.
///
/// Contains the number which identify the actual group data is transmitted
///
///////////////////////////////////////////////////////////////////////////////////////////

typedef struct group_0b {
	uint8_t PS_NAME[2];
	uint8_t PI;
	uint16_t CI : 2;
	uint16_t DI : 1;
	uint16_t MS : 1;
	uint16_t TA : 1;
	uint16_t PTY : 5;
	uint16_t TP : 1;
	uint16_t GROUP_CHAR : 1;
	uint16_t GROUP_NUM : 4;
} GROUP_0B;

///////////////////////////////////////////////////////////////////////////////////////////
///
/// \struct group_2a
/// \brief RDS Group 2A
///
/// This 2A group allows to transmit data of radiotext, with a maximum of 64 characters.\n
/// If this group is used, at least 3 groups 2A must be transmitted every 2 seconds.
///
/// \var group_2a::SEGMENT
/// \brief Radio text segments.
///
/// This for bytes contain four different characters position of them can be adressed by\n
/// B bits.
///
/// \var group_2a::B
/// \brief Address for segments.
///
/// The bits b3, b2, b1 and b0 allow to select the transmitted characters by determining\n
/// the address of the characters contained in segments.
///
/// \var group_2a::A_B
/// \brief New line indicator.
///
/// A change of state (0-> 1 or conversely) of the bit b4 A/B Flag, allows to clear the\n
/// current display to begin a new line of text.
///
/// \var group_2a::PTY
/// \brief Program Type.
///
/// The bits b5 to b9 indicate the type of program of the radio.
///
/// \var group_2a::TP
/// \brief Traffic Program.
///
/// The TP (Traffic Possible) flag indicate if program can carried Traffic information\n
/// according to the following:\n\n
///
/// 0 = This program does not carry traffic, on the other hand if TA is set, traffic\n
/// announcements can be broadcast on other program via EON\n
/// 1 = This program carries traffic announcements. If TA flag is set, a traffic\n
/// announcement is being broadcast on this program at present.
///
/// \var group_2a::GROUP_CHAR
/// \brief Group Version
///
///	0 = Group version A.\n
/// 1 = Group version B.\n
///
/// \var group_2a::GROUP_NUM
/// \brief Group Number.
///
/// Contains the number which identify the actual group data is transmitted
///
///////////////////////////////////////////////////////////////////////////////////////////

typedef struct group_2a {
	uint8_t SEGMENT[4];
	uint16_t B : 4;
	uint16_t A_B : 1;
	uint16_t PTY : 5;
	uint16_t TP : 1;
	uint16_t GROUP_CHAR : 1;
	uint16_t GROUP_NUM : 4;
} GROUP_2A;

///////////////////////////////////////////////////////////////////////////////////////////
///
/// \struct group_2b
/// \brief RDS Group 2A
///
/// This 2B group allows to transmit data of radiotext, with a maximum of 32 characters.\n
/// If this group is used, at least 6 groups 2B must be transmitted every 2 seconds.
///
/// \var group_2b::SEGMENT
/// \brief Radio text segments.
///
/// This for bytes contain four different characters position of them can be adressed by\n
/// B bits.
///
/// \var group_2b::PI
/// \brief Program Identification Code.
///
/// The Program Identification Code (PI code) allows to identify radio with country, area\n
/// coverage and program reference number
///
/// \var group_2b::B
/// \brief Address for segments.
///
/// The bits b3, b2, b1 and b0 allow to select the transmitted characters by determining\n
/// the address of the characters contained in segments.
///
/// \var group_2b::A_B
/// \brief New line indicator.
///
/// A change of state (0-> 1 or conversely) of the bit b4 A/B Flag, allows to clear the\n
/// current display to begin a new line of text.
///
/// \var group_2b::PTY
/// \brief Program Type.
///
/// The bits b5 to b9 indicate the type of program of the radio.
///
/// \var group_2b::TP
/// \brief Traffic Program.
///
/// The TP (Traffic Possible) flag indicate if program can carried Traffic information\n
/// according to the following:\n\n
///
/// 0 = This program does not carry traffic, on the other hand if TA is set, traffic\n
/// announcements can be broadcast on other program via EON\n
/// 1 = This program carries traffic announcements. If TA flag is set, a traffic\n
/// announcement is being broadcast on this program at present.
///
/// \var group_2b::GROUP_CHAR
/// \brief Group Version
///
///	0 = Group version A.\n
/// 1 = Group version B.\n
///
/// \var group_2b::GROUP_NUM
/// \brief Group Number.
///
/// Contains the number which identify the actual group data is transmitted
///
///////////////////////////////////////////////////////////////////////////////////////////

typedef struct group_2b {
	uint8_t SEGMENT[2];
	uint16_t PI;
	uint16_t B : 4;
	uint16_t A_B : 1;
	uint16_t PTY : 5;
	uint16_t TP : 1;
	uint16_t GROUP_CHAR : 1;
	uint16_t GROUP_NUM : 4;
} GROUP_2B;

///////////////////////////////////////////////////////////////////////////////////////////
///
/// \struct group_1b
/// \brief RDS Group 1B
///
/// The 1B group allows to transmit program identifications (date and the hour of the\n
/// beginning of broadcasting)
///
/// \var group_1b::PIN
/// \brief Program Item Number.
///
/// The Program Item Number PIN is the scheduled broadcast start time and day of month as
/// published by the broadcaster.
///
/// \var group_1b::PI
/// \brief Program Identification Code.
///
/// The Program Identification Code (PI code) allows to identify radio with country, area\n
/// coverage and program reference number
///
/// \var group_1b::PTY
/// \brief Program Type.
///
/// The bits b5 to b9 indicate the type of program of the radio.
///
/// \var group_1b::TP
/// \brief Traffic Program.
///
/// The TP (Traffic Possible) flag indicate if program can carried Traffic information\n
/// according to the following:\n\n
///
/// 0 = This program does not carry traffic, on the other hand if TA is set, traffic\n
/// announcements can be broadcast on other program via EON\n
/// 1 = This program carries traffic announcements. If TA flag is set, a traffic\n
/// announcement is being broadcast on this program at present.
///
/// \var group_1b::GROUP_CHAR
/// \brief Group Version
///
///	0 = Group version A.\n
/// 1 = Group version B.\n
///
/// \var group_1b::GROUP_NUM
/// \brief Group Number.
///
/// Contains the number which identify the actual group data is transmitted
///
///////////////////////////////////////////////////////////////////////////////////////////

typedef struct group_1b {
	uint16_t PIN;
	uint16_t PI_CODE;
	uint16_t B : 5;
	uint16_t PTY : 5;
	uint16_t TP : 1;
	uint16_t GROUP_CHAR : 1;
	uint16_t GROUP_NUM : 4;

} GROUP_1B;

///////////////////////////////////////////////////////////////////////////////////////////
///
/// \struct group_1a
/// \brief RDS Group 1A
///
/// The 1A group allows to transmit data of paging radio, software identifications and\n
/// program identifications (date and the hour of the beginning of broadcasting)\n
/// If it's used, this group must be transmitted at least 1 time per second.
///
/// \var group_1a::PIN
/// \brief Program Item Number.
///
/// The Program Item Number PIN is the scheduled broadcast start time and day of month as
/// published by the broadcaster.
///
/// \var group_1a::SLC
/// \brief Slow labeling Code.
///
/// Codes of software configurations ( SLC) are transmitted on the block 3 of the group\n
/// 1A, the following figure gives the meaning of this data word.
///
/// \see http://www.g.laroche.free.fr/english/rds/groupes/1/groupe1A.htm
///
/// \var group_1a::B
/// \brief Radio Paging
///
/// The following figure give the meaning for b4,b3,b2.\n
/// The b1 and b0 bits are used to synchronized and time interval (see the document\n
/// describing the RDS standard for more information).
///
/// \var group_1a::PTY
/// \brief Program Type.
///
/// The bits b5 to b9 indicate the type of program of the radio.
///
/// \var group_1a::TP
/// \brief Traffic Program.
///
/// The TP (Traffic Possible) flag indicate if program can carried Traffic information\n
/// according to the following:\n\n
///
/// 0 = This program does not carry traffic, on the other hand if TA is set, traffic\n
/// announcements can be broadcast on other program via EON\n
/// 1 = This program carries traffic announcements. If TA flag is set, a traffic\n
/// announcement is being broadcast on this program at present.
///
/// \var group_1a::GROUP_CHAR
/// \brief Group Version
///
///	0 = Group version A.\n
/// 1 = Group version B.\n
///
/// \var group_1a::GROUP_NUM
/// \brief Group Number.
///
/// Contains the number which identify the actual group data is transmitted
///
///////////////////////////////////////////////////////////////////////////////////////////

typedef struct group_1a {
	uint16_t PIN;
	uint16_t SLC;
	uint16_t B : 5;
	uint16_t PTY : 5;
	uint16_t TP : 1;
	uint16_t GROUP_CHAR : 1;
	uint16_t GROUP_NUM : 4;
} GROUP_1A;

///////////////////////////////////////////////////////////////////////////////////////////
///
/// \struct group_4a
/// \brief RDS Group 4A
///
/// The group 4A is used to transmit the hour and the date.
///
/// \var group_4a::TIME_OFF
/// \brief Local time offset.
///
/// The offset i a multiple of half hour to vary of -12h in +12h. The sign +/- is\n
/// positioned in "0" for a positve offset and "1" for a negative offset.
///
/// \var group_4a::MINUTE
/// \brief Gloabl time minute.
///
/// The minutes are coded by a binary value not signed on 6 bits and being able to vary\n
/// from 0 to 59.
///
/// \var group_4a::HOUR_L
/// \brief Low bits for gloabl hour.
///
/// The hours are coded by a binary value not signed on 5 bits and being able to vary from\n
/// 0 to 23.
///
/// \var group_4a::HOUR_H
/// \brief High bit for gloabl hour.
///
/// The hours are coded by a binary value not signed on 5 bits and being able to vary from\n
/// 0 to 23.
///
/// \var group_4a::DATE_L
/// \brief Low bits for gloabl date.
///
/// The date of day is coded on 17 bits to form a number from 0 to 99999 allowing to\n
/// determine the day, the month and the year, as well as the daytime of the week, the\n
/// number of the week and the number of the year since 1900. \n
////(See secondary G of file rbds1998.pdf for details of conversion funtions).
///
/// \var group_4a::DATE_H
/// \brief High bits for gloabl date.
///
/// The date of day is coded on 17 bits to form a number from 0 to 99999 allowing to\n
/// determine the day, the month and the year, as well as the daytime of the week, the\n
/// number of the week and the number of the year since 1900. \n
/// (See secondary G of file rbds1998.pdf for details of conversion funtions).
///
/// \var group_4a::TP
/// \brief Traffic Program.
///
/// The TP (Traffic Possible) flag indicate if program can carried Traffic information\n
/// according to the following:\n\n
///
/// 0 = This program does not carry traffic, on the other hand if TA is set, traffic\n
/// announcements can be broadcast on other program via EON\n
/// 1 = This program carries traffic announcements. If TA flag is set, a traffic\n
/// announcement is being broadcast on this program at present.
///
/// \var group_4a::PTY
/// \brief Program Type.
///
/// The bits b5 to b9 indicate the type of program of the radio.
///
/// \var group_4a::GROUP_CHAR
/// \brief Group Version
///
///	0 = Group version A.\n
/// 1 = Group version B.\n
///
/// \var group_4a::GROUP_NUM
/// \brief Group Number.
///
/// Contains the number which identify the actual group data is transmitted
///
///////////////////////////////////////////////////////////////////////////////////////////

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
	uint8_t 			align;
	SI4735_FM_RDS_ERR	err;
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
	uint16_t 			pi;
	uint8_t				fifo;
	SI4735_FM_RDS_SYNC	sync;
	SI4735_FM_RDS_INFO	info;
	SI4735_FM_STATUS 	status;
} RDS;

void rds_update(RADIO *radio);
uint8_t rds_triggered();

void rds_group_4A(RDS *data);

///////////////////////////////////////////////////////////////////////////////////////////
///
///	@}
///
///////////////////////////////////////////////////////////////////////////////////////////

#endif /* RADIO_H_ */
