/*
 * KitInfo2FW_SSP.h
 *
 *  Created on: 16/07/2012
 *      Author: Pablo
 */

#ifndef KITINFO2FW_SSP_H_
#define KITINFO2FW_SSP_H_

#include "fw.h"
#include "sysdefs.h"

//Prototipos
void ssp_ssel_set_low();
void ssp_ssel_set_high();
void ssp_setup(void);
void ssp_set_clock(uint32_t clk);
void ssp_set_clock_slow();
void ssp_set_clock_fast();
void ssp_send_data(uint16_t dato);
uint16_t ssp_get_data();


#endif /* KITINFO2FW_SSP_H_ */
