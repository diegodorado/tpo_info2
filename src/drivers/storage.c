/*
 * storage.c
 *
 *  Created on: Sep 19, 2014
 *      Author: diego
 */

#include "storage.h"
#include "drivers.h"

static volatile uint32_t block_index=0; ///needed?
static volatile status_hdr_t status;
static volatile storage_sd_status_t sd_status;

static volatile uint8_t card_detected;
static volatile uint8_t last_card_detected;
static void storage_debounce_card_detected(void);


void storage_setup(void)
{

  set_pin_sel( STORAGE_CARD_DETECT_PIN,0); // as gpio
  gpio_set_dir(STORAGE_CARD_DETECT_PIN,0); // as input
  set_pin_mode(STORAGE_CARD_DETECT_PIN,0); // with pull up resistor

  // get initial values (not debounced though)
  card_detected = last_card_detected = gpio_get_pin(STORAGE_CARD_DETECT_PIN,0);
  //debounce every 200ms
  systick_delay_async(200, 1,storage_debounce_card_detected);

  storage_boot_sd();

}



static void storage_debounce_card_detected(void){
  // read if CD to ground
  uint8_t current_card_detected = gpio_get_pin(STORAGE_CARD_DETECT_PIN,0);

  //check if value changed and kept its value on subsequent tick
  if (current_card_detected != card_detected  && current_card_detected == last_card_detected )
    card_detected  = current_card_detected;

  // memoize value
  last_card_detected  = current_card_detected;
}



void storage_boot_sd(void){

  uint8_t buf[FILECHUNK_SIZE];
  uint8_t* buf_ptr = buf;
  int i;

  status.blocks_count = 0;
  status.files_count = 0;


  if ( !sd_card_setup() ){
    sd_status = SD_STATUS_SETUP_FAILURE;
    return;
  }


  if ( !sd_card_read(buf_ptr, 1, STORAGE_DISK_HEADER_BLOCK ) )
  {
    sd_status = SD_STATUS_READ_FAILURE;
    return;
  }

  if ( *( (uint32_t*) buf_ptr) != STORAGE_DISK_HEADER_HEAD)
  {
    sd_status = SD_STATUS_WRONG_FORMAT;
    return;
  }

  // avanzo el buffer
  buf_ptr+= sizeof(STORAGE_DISK_HEADER_HEAD);

  for (i = 0; i < sizeof(status_hdr_t); i++)
    *((uint8_t*) &status + i) = *buf_ptr++;

  if ( *( (uint32_t*) buf_ptr) != STORAGE_DISK_HEADER_TAIL){
    sd_status = SD_STATUS_WRONG_FORMAT;
    return;
  }

  status.blocks_count = sd_card_size();

  if(status.blocks_count == 0){
    sd_status = SD_STATUS_INVALID_SIZE;
    return;
  }

  sd_status = SD_STATUS_OK; //overwrite what was read from sd


}

int storage_card_detected(){
  return card_detected;
}

storage_sd_status_t storage_sd_status(){
  return sd_status;
}

uint8_t  storage_sd_files_count(){
  return status.files_count;
}

uint32_t storage_sd_blocks_count(){
  return status.blocks_count;
}

uint32_t storage_sd_last_block(){
  return status.last_block;
}




void storage_format_sd(void)
{
  uint32_t last_block = STORAGE_FILE_HEADERS_START_BLOCK + STORAGE_FILE_HEADERS_BLOCKS_COUNT;
  storage_write_header(0 , last_block );
}



int storage_write_header(uint32_t files_count,uint32_t last_block)
{
  uint8_t buf[FILECHUNK_SIZE];
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

  if ( !sd_card_write(buf, 1, STORAGE_DISK_HEADER_BLOCK ) )
    return (sd_status = SD_STATUS_WRITE_FAILURE);

  return 0;
}

int storage_get_file_headers(fileheader_data_t * file_headers, uint8_t files_count)
{

  if ( !sd_card_read( (uint8_t*) file_headers, STORAGE_FILE_HEADERS_BLOCKS_COUNT, STORAGE_FILE_HEADERS_START_BLOCK) )
    return (sd_status = SD_STATUS_READ_FAILURE);

  return 0; // no errors

}


int storage_save_file_header(fileheader_data_t * file_header)
{

  fileheader_data_t file_headers[STORAGE_FILE_HEADERS_MAX_COUNT];
  uint8_t file_headers_index;

  if( storage_get_file_headers(file_headers, status.files_count)!=0)
    return sd_status;

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



