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
#include "protocol.h"

void storage_setup(void);

#define STORAGE_CARD_DETECT_PIN 2,8 //pin P2.8
#define STORAGE_DISK_HEADER_HEAD 0xF0A9B8C7 //una combinacion cualquiera
#define STORAGE_DISK_HEADER_TAIL 0xF63A2B1C //para comprobar que el disco esta formateado

#define STORAGE_DISK_HEADER_BLOCK 0
#define STORAGE_FILE_HEADERS_START_BLOCK 1
#define STORAGE_FILE_HEADERS_BLOCKS_COUNT 8
#define STORAGE_FILE_HEADERS_MAX_COUNT 128


typedef enum {
  SD_STATUS_OK,
  SD_STATUS_NOT_DETECTED,
  SD_STATUS_SETUP_FAILURE,
  SD_STATUS_READ_FAILURE,
  SD_STATUS_WRITE_FAILURE,
  SD_STATUS_WRONG_FORMAT,
  SD_STATUS_INVALID_SIZE,
} storage_sd_status_t;


int storage_card_detected();
storage_sd_status_t storage_sd_status();
uint8_t  storage_sd_files_count();
uint32_t storage_sd_blocks_count();
uint32_t storage_sd_last_block();
status_hdr_t storage_status();

void storage_boot_sd(void);
void storage_format_sd(void);
void storage_write_header(uint8_t files_count,uint32_t last_block);
void storage_get_file_headers(fileheader_data_t * file_headers);
void storage_save_file_header(fileheader_data_t file_header);

#endif /* STORAGE_H_ */
