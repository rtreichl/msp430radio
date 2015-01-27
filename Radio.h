/*
 * Radio.h
 *
 *  Created on: 21.01.2014
 *      Author: Richi
 */

#ifndef RADIO_H_
#define RADIO_H_

#include <stdint.h>
#define RDS_BYTES_OFFSET 2

void get_rds_data(int *Radio_States, char *Station_Name, char *Radion_Text);
uint8_t rds_triggered();
void rds_group_4A(char *rds_data);

#endif /* RADIO_H_ */
