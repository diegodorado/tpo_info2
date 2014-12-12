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

#define STORAGE_DISK_HEADER_HEAD 0xF0A9B8C7 //una combinacion cualquiera
#define STORAGE_DISK_HEADER_TAIL 0xF63A2B1C //para comprobar que el disco esta formateado

#define STORAGE_DISK_HEADER_BLOCK 0
#define STORAGE_FILE_HEADERS_START_BLOCK 1
#define STORAGE_FILE_HEADERS_BLOCKS_COUNT 8
#define STORAGE_FILE_HEADERS_MAX_COUNT 128

void storage_disk_status(status_hdr_t*);
int  storage_format_disk(void);
int storage_write_header(uint32_t files_count,uint32_t last_block);
int storage_get_file_headers(fileheader_data_t * file_headers, uint8_t files_count);
int storage_save_file_header(fileheader_data_t * file_header);

#endif /* STORAGE_H_ */
