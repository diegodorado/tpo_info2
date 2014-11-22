/*
 * storage.h
 *
 *  Created on: Sep 19, 2014
 *      Author: diego
 */

#ifndef STORAGE_H_
#define STORAGE_H_

//include firmware
#include "fw.h"

void storage_setup(void);

#define N_BLOCKS 5
#define START_BLOCK 0
uint8_t sd_card_test(void);
uint8_t sd_card_write_test(char * text);
uint8_t sd_card_read_test(char * text, uint8_t size);


#endif /* STORAGE_H_ */
