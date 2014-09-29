/*
 * client.h
 *
 *  Created on: Sep 19, 2014
 *      Author: diego
 */

#ifndef CLIENT_H_
#define CLIENT_H_

//include firmware
#include "fw.h"
#include "drivers.h"



/**
 * TRAMA SERIE
 * [(tiempo)#velocidad#%evento%checksum]
 * evento: 0 a 4 (numero de pulsador)
 * ejemplo: [(22:39)#87#%numero_pulsador%xor_total]
 *
 * */
typedef struct{
  uint8_t minutes;
  uint8_t seconds;
  uint8_t velocity;
  uint8_t event;
  uint8_t checksum;
} tp4_data_frame_t;


void client_setup(void);

uint8_t client_rx_pop ( void);
void client_tx_push ( uint8_t);
uint8_t client_data_frame_received (void);

void client_send_data_frame (uint8_t,uint8_t, uint8_t);
tp4_data_frame_t client_decode_data_frame(void);
uint8_t client_is_checksum_ok(tp4_data_frame_t);

#endif /* CLIENT_H_ */
