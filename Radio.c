/*
 * Radio.c
 *
 *  Created on: 21.01.2014
 *      Author: Richi
 */

#include "Radio.h"
#include "MSP430G2553_USCI_I2C.h"
#include "SI4735.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "Timer.h"
#include "Menu.h"

void get_rds_data(int *Radio_States, char *Station_Name, char *Radion_Text)                 //nur Sender-Stationsnamen auslesen
{
	//Radio States => [New Freq Bit, Station_Present Bit, Radio_Text_Update Bit, 0 Bit, TP Bit, PTY 5 Bits, TA Bit, M/S Bit, DI 4 Bits] LSB
	char GRP, pos;
	char rds_read_byte[14];
	char tmp = 0, doit = 0;
	static char rds_text_count = 0, rds_station_count = 0;
	if(*Radio_States & (1<<15))
	{
		rds_text_count = 0;
		rds_station_count = 0;
		*Radio_States &= ~0xF000;
	}
	if(rds_triggered() & 0x04)
	{
		do
		{
			USCI_I2C_WRITE2(I2C_SI4735, REPT, 2, 0x24, 0x01);
			USCI_I2C_READ(I2C_SI4735, STOP, 13, rds_read_byte);
			I2C_write_and_read(2,i2c_data_out,14,rds_read_byte);                             //RDS-Daten und Status auslesen
			if(!((rds_read_byte[11 + RDS_BYTES_OFFSET] == 3) || (rds_read_byte[11 + RDS_BYTES_OFFSET] == 12) || (rds_read_byte[11 + RDS_BYTES_OFFSET] == 48) || (rds_read_byte[11 + RDS_BYTES_OFFSET] == 192)) && rds_read_byte[0] == 0)
			if(rds_read_byte[2 + RDS_BYTES_OFFSET]>0)                       //neue Daten empfangen?
			{
				GRP = (rds_read_byte[5 + RDS_BYTES_OFFSET]&0xF0)>>4;
				if (GRP == 2)
				{
					pos = (rds_read_byte[6 + RDS_BYTES_OFFSET] & 0x0F)<<2;
					if(++rds_text_count == 16)
					{
						rds_text_count = 0;
						*Radio_States |= (1<<13);
					}
					Radion_Text[pos++] = rds_read_byte[7 + RDS_BYTES_OFFSET];
					Radion_Text[pos++] = rds_read_byte[8 + RDS_BYTES_OFFSET];
					Radion_Text[pos++] = rds_read_byte[9 + RDS_BYTES_OFFSET];
					Radion_Text[pos] = rds_read_byte[10 + RDS_BYTES_OFFSET];
					Radion_Text[65] = '\0';
				}
				if (GRP == 4)
				{
					rds_group_4A(rds_read_byte);
				}
				if (GRP == 0)
				{

					*Radio_States |= (rds_read_byte[6 + RDS_BYTES_OFFSET] & 0xFC)>>2;	//TA Bit M/S Bit DI 4 Bits
					pos = (rds_read_byte[6 + RDS_BYTES_OFFSET] & 0x03);               	//Adresse herausfiltern
					pos = pos<<1;
					Station_Name[pos] = rds_read_byte[9 + RDS_BYTES_OFFSET];
					pos = pos + 1;
					Station_Name[pos] = rds_read_byte[10 + RDS_BYTES_OFFSET];
					Station_Name[8] = '\0';
					if(++rds_station_count == 4)
					{
						rds_station_count = 0;
						*Radio_States |= (1<<14);
					}
				}
			}
			_delay_us(25);
			//_delay_ms(1);
			tmp = rds_read_byte[2 + RDS_BYTES_OFFSET];
			if(tmp < 20 && doit == 0)
			{
				break;
			}
			else
			{
				doit = 1;
			}
		}
		while(tmp > 0);//rds_read_byte[2 + RDS_BYTES_OFFSET] > 0);
	}
	*Radio_States |= (rds_read_byte[5 + RDS_BYTES_OFFSET] & 0x07)<<9 | (rds_read_byte[6 + RDS_BYTES_OFFSET] & 0xE0)<<1; //TP Bit PTY 5 Bits
uint8_t get_signal_qual(uint8_t *stats)
{
	USCI_I2C_WRITE2(I2C_SI4735, REPT, 2, 0x23, 0x00);
	USCI_I2C_READ(I2C_SI4735, STOP, 8, stats);
	return 0;
}

uint8_t rds_triggered()
{
	uint8_t rds = 0;
	USCI_I2C_WRITE2(I2C_SI4735, REPT, 1, 0x14);
	USCI_I2C_READ(I2C_SI4735, STOP, 1, &rds);
	return rds;
}

void rds_group_4A(char *rds_data)
{
	char offset = 0;
	unsigned int mdj = 0;
	unsigned char m_hour, m_minute, m_day, m_month, m_year;
	unsigned char day_per_month[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	int temp;

	m_day = 1;
	m_month = 0;
	m_year = 10;

	m_minute = (rds_data[9 + RDS_BYTES_OFFSET] & 0x0F)<<2;
	m_minute |= (rds_data[10 + RDS_BYTES_OFFSET] & 0xC0)>>6;
	m_hour = (rds_data[8 + RDS_BYTES_OFFSET] & 0x01)<<4;
	m_hour |= (rds_data[9 + RDS_BYTES_OFFSET] & 0xF0)>>4;
	offset = (rds_data[10 + RDS_BYTES_OFFSET] & 0x1F);
	mdj = (rds_data[8 + RDS_BYTES_OFFSET] & 0xFE)>>1;
	mdj |= (rds_data[7 + RDS_BYTES_OFFSET])<<7;
	mdj |= (rds_data[6 + RDS_BYTES_OFFSET] & 0x03)<<15;

	temp = mdj - 55198 + 1; //55198 = MDJ of 01.01.2010

	while(temp > 0)
	{
		m_day = temp;
		m_month++;
		if (m_month == 13)
		{
			m_month = 1;
			m_year++;
		}
		temp -= day_per_month[m_month-1];
	}
	temp = m_minute + 60 * m_hour + 30 * offset;
	if (temp >= 1440)
	{
		temp -= 1440;
		m_day++;
	}
	if (temp < 0)
	{
		temp += 1440;
		m_day--;
	}
	m_minute = temp%60;
	m_hour = temp/60;
	time_date(m_hour, m_minute, m_day, m_month, m_year, 1, 0, 0, 0);
}
