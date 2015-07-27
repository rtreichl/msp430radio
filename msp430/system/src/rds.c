/*
 * Radio.c
 *
 *  Created on: 21.01.2014
 *      Author: Richi
 */

#include <system/rds.h>
#include <string.h>

//----------------------------------------------------------------------------------------
//
/// \brief Updates the current RDS data\n
///
///	(1)Check if new rds data has arrived\n
///	(2)Update the rds data in RADIO object
//
/// \param	<radio>	[out]	Radio status with RDS data
//
/// \retval void
//
/// \remarks
//
//----------------------------------------------------------------------------------------

void rds_update(RADIO *radio)                 //nur Sender-Stationsnamen auslesen
{
	uint8_t pos;
	RDS rds = {0};
	uint8_t doit = 0;
	static uint8_t rds_text_count = 0, rds_station_count = 0;
	if(radio->status.freq_valid == RADIO_NOT_VALID)
	{
		rds_text_count = 0;
		rds_station_count = 0;
		radio->status.name_valid = RADIO_NOT_VALID;
		radio->status.text_valid = RADIO_NOT_VALID;
		radio->status.freq_valid = RADIO_VALID;
	}
	if(rds_triggered() & RDS_INTERRUPT) {
		do {
			i2c_write_var(I2C_SI4735, REPT, 2, 0x24, 0x01);
			i2c_read(I2C_SI4735, STOP, I2C_BIG_ENDIAN, 13, &rds);
			if(rds.err.BLEA != RDS_BLOCK_ERROR_UNCORRECTABLE &&
			   rds.err.BLEB != RDS_BLOCK_ERROR_UNCORRECTABLE &&
			   rds.err.BLEC != RDS_BLOCK_ERROR_UNCORRECTABLE &&
			   rds.err.BLED != RDS_BLOCK_ERROR_UNCORRECTABLE) {
				switch(rds.block_b.GROUP_NUM) {
				case RDS_GROUP_NUM_2:
					pos = rds.group_2a.B * RDS_RADIO_TEXT_SYMBOLS_PER_FRAME;
					if(++rds_text_count == RDS_RADIO_TEXT_MAX_SYMBOLS/RDS_RADIO_TEXT_SYMBOLS_PER_FRAME) {
						rds_text_count = 0;
						radio->status.text_valid = RADIO_VALID;
					}
					radio->rds.text[pos++] = rds.group_2a.SEGMENT[3];
					radio->rds.text[pos++] = rds.group_2a.SEGMENT[2];
					radio->rds.text[pos++] = rds.group_2a.SEGMENT[1];
					radio->rds.text[pos] = rds.group_2a.SEGMENT[0];
					radio->rds.text[64] = '\0';
					break;
				case RDS_GROUP_NUM_4:
					rds_group_4A(&rds);
					break;
				case RDS_GROUP_NUM_0:
					radio->rds.di &= ~(1 << rds.group_0a.CI);
					radio->rds.di |= ~((1 & rds.group_0a.DI)<< rds.group_0a.CI);
					radio->rds.ms = rds.group_0a.MS;
					radio->rds.ta = rds.group_0a.TA;
					pos = rds.group_0a.CI * RDS_STATION_NAME_SYMBOLS_PER_FRAME;
					radio->rds.name[pos++] = rds.group_0a.PS_NAME[1];
					radio->rds.name[pos] = rds.group_0a.PS_NAME[0];
					radio->rds.name[8] = '\0';
					if(++rds_station_count == RDS_STATION_NAME_MAX_SYMBOLS/RDS_STATION_NAME_SYMBOLS_PER_FRAME) {
						rds_station_count = 0;
						radio->status.name_valid = RADIO_VALID;
					}
					break;
				}
			}
			radio->rds.pty = rds.block_b.PTY;
			radio->rds.pi = rds.pi;
			radio->rds.tp = rds.block_b.TP;
			_delay_ten_us(5);
			if(rds.fifo.RDSFIFOUSED < RDS_FIFO_MAX_SIZE && doit == 0) {
				break;
			}
			else {
				doit = 1;
			}
		}
		while(rds.fifo.RDSFIFOUSED > 0);
	}
}

//----------------------------------------------------------------------------------------
//
/// \brief Triggers RDS
//
/// \param	void
//
/// \retval uint8_t
//
/// \remarks
//
//----------------------------------------------------------------------------------------

uint8_t rds_triggered()
{
	uint8_t rds = 0;
	i2c_write_var(I2C_SI4735, REPT, 1, 0x14);
	i2c_read(I2C_SI4735, STOP, I2C_LITTLE_ENDIAN, 1, &rds);
	return rds;
}

//----------------------------------------------------------------------------------------
//
/// \brief Gets the time and date out of RDS stream
//
/// \param	<data>	[in]	RDS data to be converted
//
/// \retval	void
//
/// \remarks
//
//----------------------------------------------------------------------------------------

void rds_group_4A(RDS *rds)
{
	uint8_t offset = 0;
	int16_t mdj = 0;
	uint8_t m_hour, m_minute, m_day, m_month, m_year;

	m_day = 1;
	m_month = 0;
	m_year = 10;

	m_minute = rds->group_4a.MINUTE;
	m_hour = rds->group_4a.HOUR_L + rds->group_4a.HOUR_H * 16;
	offset = rds->group_4a.TIME_OFF;
	mdj = rds->group_4a.DATE_L + (rds->group_4a.DATE_H << 15);

	mdj = mdj - 55198 + 1; //55198 = MDJ of 01.01.2010

	while(mdj > 0) {
		m_day = mdj;
		m_month++;

		if (m_month == 13) {
			m_month = 1;
			m_year++;
		}

		mdj -= month_days[m_month - 1];
	}

	mdj = m_minute + 60 * m_hour + 30 * offset;

	if (mdj >= 1440) {
		mdj -= 1440;
		m_day++;
	}

	if (mdj < 0) {
		mdj += 1440;
		m_day--;
	}

	m_minute = mdj % 60;
	m_hour = mdj / 60;
	time_set_time(m_hour, m_minute, m_day, m_month, m_year, 0);
}
