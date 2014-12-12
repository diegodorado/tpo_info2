/*
 * storage.c
 *
 *  Created on: Sep 19, 2014
 *      Author: diego
 */

#include "storage.h"



static volatile uint8_t buffer[512];
static volatile uint16_t buffer_in=0;
static volatile uint16_t buffer_out=0;
static volatile uint32_t block_index=0;


void storage_setup(void)
{
  //todo: implementar la inicializacion
}


void storage_disk_status(status_hdr_t* status)
{
  uint8_t buf[512];
  uint8_t* buf_ptr = buf;
  int i;

  if ( !sd_card_setup() )
  {
    status->sd_status = 1;
    return;
  }

  if ( !sd_card_read(buf_ptr, 1, STORAGE_DISK_HEADER_BLOCK ) )
  {
    status->sd_status = 2;
    return;
  }

  if ( *( (uint32_t*) buf_ptr) != STORAGE_DISK_HEADER_HEAD)
  {
    status->sd_status = 3;
    return;
  }

  // avanzo el buffer
  buf_ptr+= sizeof(STORAGE_DISK_HEADER_HEAD);

  for (i = 0; i < sizeof(status_hdr_t); i++)
    *((uint8_t*) status + i) = *buf_ptr++;

  status->sd_status = 0; //overwrite what was read from sd

  if ( *( (uint32_t*) buf_ptr) != STORAGE_DISK_HEADER_TAIL)
    status->sd_status = 3;



}


int storage_format_disk(void)
{
  uint32_t last_block = STORAGE_FILE_HEADERS_START_BLOCK + STORAGE_FILE_HEADERS_BLOCKS_COUNT - 1;
  return storage_write_header(0 , last_block );
}



int storage_write_header(uint32_t files_count,uint32_t last_block)
{
  uint8_t buf[512];
  uint8_t* buf_ptr = buf;
  status_hdr_t status;
  int i;

  status.files_count = files_count;
  status.last_block = last_block;

  *( (uint32_t*) buf_ptr) = STORAGE_DISK_HEADER_HEAD;

  // avanzo el buffer
  buf_ptr+= sizeof(STORAGE_DISK_HEADER_HEAD);

  for (i = 0; i < sizeof(status_hdr_t); i++)
    *buf_ptr++ = *((uint8_t*) &status + i);

  *( (uint32_t*) buf_ptr) = STORAGE_DISK_HEADER_TAIL;

  if ( !sd_card_setup() )
    return 1;

  if ( !sd_card_write(buf, 1, STORAGE_DISK_HEADER_BLOCK ) )
    return 2;

  return 0; //no errors

}


int storage_get_file_headers(fileheader_data_t * file_headers, uint8_t files_count)
{

  if ( !sd_card_setup() )
    return 1; //error 1
  else if ( !sd_card_read( (uint8_t*) file_headers, STORAGE_FILE_HEADERS_BLOCKS_COUNT, STORAGE_FILE_HEADERS_START_BLOCK) )
    return 2; // error 2

  return 0; // no errors

}


int storage_save_file_header(fileheader_data_t * file_header)
{

  status_hdr_t status;
  fileheader_data_t file_headers[STORAGE_FILE_HEADERS_MAX_COUNT];
  uint8_t file_headers_index;

  storage_disk_status(&status);

  if( status.sd_status!=0)
    return 1; //error 1

  if( storage_get_file_headers(file_headers, status.files_count)!=0)
    return 2; //error 2

  file_header->block_start = status.last_block;
  status.last_block += file_header->chunks_count;
  status.files_count++;

  // save the new disk header
  if (storage_write_header(status.files_count , status.last_block )!=0)
    return 3; //error 3

  file_headers_index = status.files_count / 16;

  if ( !sd_card_write((uint8_t*)&file_headers[file_headers_index], 1, STORAGE_FILE_HEADERS_START_BLOCK + file_headers_index ) )
    return 4; // error 4

  return 0; // no errors

}



