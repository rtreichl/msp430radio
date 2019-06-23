///----------------------------------------------------------------------------------------
///
/// \file rds.c
///
/// \brief rds.c get values from si4735 and extract rds infromation
///
/// \date 21.01.2014
///
/// \author Richard Treichl
///
/// \remark http://www.g.laroche.free.fr/english/rds/groupes/tramesRDS.htm
///
/// \todo
///
/// \version	1.0
///
///----------------------------------------------------------------------------------------

#include <system/rds.h>
#include <string.h>

///----------------------------------------------------------------------------------------
///
/// \brief Updates the current RDS data\n
///
///	(1)Check if new rds data are available\n
///	(2)Check each data block for errors\n
/// (3)Detect rds group and execute part for decoding\n
/// \n
/// If fifo size greater than trigger, the loop is only running until zero data are
///	available in the fifo.
///
/// \param	<radio>	[out]	Radio status with RDS data
///
/// \retval void
///
/// \remarks
///
///----------------------------------------------------------------------------------------

void rds_update(RADIO *radio)                 //nur Sender-Stationsnamen auslesen
{
	uint8_t pos;
	RDS rds = {0};
	uint8_t doit = 0;
	static uint8_t rds_text_count = 0, rds_station_count = 0;

	/* If radio frequency not longer valid reset all working variables */
	if(radio->status.freq_valid == RADIO_NOT_VALID)
	{
		rds_text_count = 0;
		rds_station_count = 0;
		radio->status.name_valid = RADIO_NOT_VALID;
		radio->status.text_valid = RADIO_NOT_VALID;
		radio->status.freq_valid = RADIO_VALID;
	}
	/* Trigger for rds interrupt it is one when new data are available */
	if(rds_triggered() & RDS_INTERRUPT) {
		do {
			i2c_write_var(I2C_SI4735, REPT, 2, 0x24, 0x01);
			i2c_read(I2C_SI4735, STOP, I2C_BIG_ENDIAN, 13, &rds);
			/* Check for block errors only if there are only correctable data got for ward else skip this block */
			if(rds.err.BLEA != RDS_BLOCK_ERROR_UNCORRECTABLE &&
			   rds.err.BLEB != RDS_BLOCK_ERROR_UNCORRECTABLE &&
			   rds.err.BLEC != RDS_BLOCK_ERROR_UNCORRECTABLE &&
			   rds.err.BLED != RDS_BLOCK_ERROR_UNCORRECTABLE) {
				/* Detect rds group number for given data set */
				switch(rds.block_b.GROUP_NUM) {
				case RDS_GROUP_NUM_2:
					pos = rds.group_2a.B * RDS_RADIO_TEXT_SYMBOLS_PER_FRAME;
					/* if 64 symbols received indecate that radio text is now valid */
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
					/* if 8 symbols received indecate that station name is now valid */
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
			/* if fifo puffer less than RDS_FIFO_MAX_SIZE jump out of loop else read every data from rds fifo until it is zero */
			if(rds.fifo < RDS_FIFO_MAX_SIZE && doit == 0) {
				break;
			}
			else {
				doit = 1;
			}
		}
		while(rds.fifo > 0);
	}
}

///----------------------------------------------------------------------------------------
///
/// \brief Get the actuall state of si4735 to have a look if rds int is triggerd
///
/// \param	void
///
/// \retval uint8_t
///
/// \remarks
///
///----------------------------------------------------------------------------------------

uint8_t rds_triggered()
{
	uint8_t rds = 0;
	i2c_write_var(I2C_SI4735, REPT, 1, 0x14);
	i2c_read(I2C_SI4735, STOP, I2C_LITTLE_ENDIAN, 1, &rds);
	return rds;
}

///----------------------------------------------------------------------------------------
///
/// \brief Calculate the time and date out of RDS information
///
/// \param	<rds>	[in]	pointer to the orignal rds variable
///
/// \retval	void
///
/// \remarks
///
///----------------------------------------------------------------------------------------
void rds_group_4A(RDS *rds)
{
	uint8_t offset = 0;
	static int32_t mjd = 0;
	uint8_t m_hour, m_minute, m_day, m_month;
	uint16_t m_year;

	//Read all values
	m_minute = rds->group_4a.MINUTE;
	m_hour = rds->group_4a.HOUR_L + rds->group_4a.HOUR_H * 16;
	offset = rds->group_4a.TIME_OFF;
	mjd = ((uint32_t)(rds->group_4a.DATE_L)) + ((uint32_t)(rds->group_4a.DATE_H) << 15);

	//Init variables for date calculation
	m_day = 1;
	m_month = 3;
	m_year = 1900;
	//Subtract 15078 which is equal to 01.03.1900
	mjd -= 15078;
	//Multiple with 4 to do this operation with out float
	//mjd /= 365,25
	mjd = mjd << 2;

	//Count years from 1900 until the final year is reached
	while (mjd > 1461) {
		mjd -= 1461;
		m_year++;
	}

	//Multiple with 8 end up at a multiplication of 32 for next operation
	//mdj /= 30,6001
	mjd = mjd << 3;

	//Count month from 3 until the final month is reached
	while (mjd > 979) {
		mjd -= 979;
		if (++m_month == 13) {
			m_month = 1;
			m_year++;
		}
	}
	//Final day is the rest dived by 32
	m_day += (mjd >> 5);

	//Sum up all minutes to see if there is a day shift
	mjd = m_minute + 60 * m_hour + 30 * offset;

	//If there more minutes than a day has increase day by one and subtract minutes
	if (mjd >= 1440) {
		mjd -= 1440;
		m_day++;
	}

	//If there are less than 0 minutes decrease day by one and add minutes
	if (mjd < 0) {
		mjd += 1440;
		m_day--;
	}

	//Do this operation to get last two digits
	m_year = m_year % 100;
	//Get new minutes back
	m_minute = mjd % 60;
	//Get new hours back
	m_hour = mjd / 60;
	//Update time of system
	time_set_time(m_hour, m_minute, m_day, m_month, m_year, 0);
}
