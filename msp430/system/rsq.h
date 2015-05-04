/*
 * rsq.h
 *
 *  Created on: 04.04.2015
 *      Author: Richard
 */

//----------------------------------------------------------------------------------------
//
/// \brief In here the function to update Radio Signal Quality
//
//----------------------------------------------------------------------------------------

#ifndef SYSTEM_SRC_RSQ_H_
#define SYSTEM_SRC_RSQ_H_

#include <stdint.h>
#include <driver/si4735.h>
#include <driver/i2c.h>
#include <radio/radio.h>

uint8_t rsq_update(RADIO *radio);

#endif /* SYSTEM_SRC_RSQ_H_ */
