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


typedef struct {
  uint8_t files_count;
  uint32_t disk_space;
  uint32_t available_space;
} storage_disk_header_t;


typedef struct {
 char filename[8];
 uint32_t length;
 uint16_t block_index; // indice de bloque de la SD donde comienza el audio del archivo
 uint8_t sample_rate;  // esto vale 0 si es 8000 samples por segundo .. etc
 uint8_t RESERVED0;    // para alinear la estructura en 16 bytes
} storage_file_header_t;

void storage_setup(void);

#define FILE_HEADERS_COUNT 128
#define FILE_HEADER_BLOCKS_COUNT 8
#define N_BLOCKS 5
#define START_BLOCK 0

uint8_t sd_card_test(void);
uint8_t sd_card_write_test(char * text);
uint8_t sd_card_read_test(char * text, uint8_t size);


#endif /* STORAGE_H_ */
