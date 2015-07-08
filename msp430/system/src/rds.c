/*
 * Radio.c
 *
 *  Created on: 21.01.2014
 *      Author: Richi
 */

#include <system/rds.h>

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
	//Radio States => [New Freq Bit, Station_Present Bit, Radio_Text_Update Bit, 0 Bit, TP Bit, PTY 5 Bits, TA Bit, M/S Bit, DI 4 Bits] LSB
	uint8_t pos;
	uint8_t rds_read_byte[13];
	uint8_t i;
	uint8_t temp;
	RDS *rds = (RDS*) (uint16_t*) rds_read_byte;
	char tmp = 0, doit = 0;
	static uint8_t rds_text_count = 0, rds_station_count = 0;
	if(radio->status.freq_valid == RADIO_NOT_VALID)
	{
		rds_text_count = 0;
		rds_station_count = 0;
		radio->status.name_valid = RADIO_NOT_VALID;
		radio->status.text_valid = RADIO_NOT_VALID;
		radio->status.freq_valid = RADIO_VALID;
	}

	//TODO: replace 0x04 with macro
	if(rds_triggered() & 0x04)
	{
		do
		{
			i2c_write_var(I2C_SI4735, REPT, 2, 0x24, 0x01);
			i2c_read(I2C_SI4735, STOP, 13, rds_read_byte);
			for (i = 4; i <= 10; i+=2) {
				temp = rds_read_byte[i];
				rds_read_byte[i] = rds_read_byte[i+1];
				rds_read_byte[i+1] = temp;
			}
			//TODO: replace numbers
			if(rds->err.BLEA != 3 && rds->err.BLEB != 3 && rds->err.BLEC != 3 && rds->err.BLED != 3)
			{
				//TODO:replace number
				if (rds->block_b.GROUP_NUM == 2)
				{
					GROUP_2A *rds2 = (GROUP_2A*) &(rds->pi);
					pos =rds2->B*4;
					//TODO:replace number
					if(++rds_text_count == 16)
					{
						rds_text_count = 0;
						radio->status.text_valid = RADIO_VALID;
					}
					radio->rds.text[pos++] = rds2->SEGMENT[1];
					radio->rds.text[pos++] = rds2->SEGMENT[0];
					radio->rds.text[pos++] = rds2->SEGMENT[3];
					radio->rds.text[pos] = rds2->SEGMENT[2];
					radio->rds.text[64] = '\0';
				}
				//TODO:replace number
				if (rds->block_b.GROUP_NUM == 4)
				{
					rds_group_4A(rds);
				}
				//TODO:replace number
				if (rds->block_b.GROUP_NUM == 0)
				{

					//*Radio_States |= (rds_read_byte[6 + RDS_BYTES_OFFSET] & 0xFC)>>2;	//TA Bit M/S Bit DI 4 Bits
					GROUP_0A *rds2 = (GROUP_0A*) &(rds->pi);

					radio->rds.di &= ~(1 << rds2->CI);
					radio->rds.di |= ~((1 & rds2->DI)<< rds2->CI);

					radio->rds.ms = rds2->MS;
					radio->rds.ta = rds2->TA;

					//*Radio_States |= (rds2->TA | rds2->MS | rds2->DI) >> 2;
					pos = rds2->CI*2;
					radio->rds.name[pos++] = rds2->PS_NAME[1];
					radio->rds.name[pos] = rds2->PS_NAME[0];
					radio->rds.name[8] = '\0';
					//TODO:replace number
					if(++rds_station_count == 4)
					{
						rds_station_count = 0;
						radio->status.name_valid = RADIO_VALID;
					}
				}
			}
			radio->rds.pty = rds->block_b.PTY;
			radio->rds.pi = rds->pi;
			radio->rds.tp = rds->block_b.TP;
			_delay_ten_us(5);
			tmp = rds->fifo.RDSFIFOUSED;
			//TODO:replace numbers
			if(tmp < 10 && doit == 0)
			{
				break;
			}
			else
			{
				doit = 1;
			}
		}
		while(tmp > 0);
	}
	//*Radio_States |= (rds_read_byte[5 + RDS_BYTES_OFFSET] & 0x07)<<9 | (rds_read_byte[6 + RDS_BYTES_OFFSET] & 0xE0)<<1; //TP Bit PTY 5 Bits
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
	i2c_read(I2C_SI4735, STOP, 1, &rds);
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

void rds_group_4A(RDS *data)
{
	GROUP_4A *group_4a = (GROUP_4A*) &(data->pi);
	uint8_t offset = 0;
	int16_t mdj = 0;
	uint8_t m_hour, m_minute, m_day, m_month, m_year;

	m_day = 1;
	m_month = 0;
	m_year = 10;

	m_minute = group_4a->MINUTE;
	m_hour = group_4a->HOUR_L + group_4a->HOUR_H * 16;
	offset = group_4a->TIME_OFF;
	mdj = group_4a->DATE_L + (group_4a->DATE_H << 15);

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
