/*
 * rsq.c
 *
 *  Created on: 04.04.2015
 *      Author: Richard
 */

#include <system/rsq.h>

//----------------------------------------------------------------------------------------
//
/// \brief	Update the Radio Signal Quality
//
/// When updating rsq following steps are required:\n
///	(1)Get the current rsq status\n
///	(2)Update rsq
//
/// \param	<radio>		[out]	Current radio rsq state
//
/// \retval
//
/// \remarks
//
//---------------------------------------------------------------------------------------

uint8_t rsq_update(RADIO *radio)
{
	uint8_t resp[8];
	si4735_fm_rsq_status(1, resp);

	radio->rsq.rssi = resp[4];
	radio->rsq.snr = resp[5];
	radio->rsq.multi = resp[6];
	radio->rsq.freq_off = (int8_t)resp[7];

	return 0;
}
