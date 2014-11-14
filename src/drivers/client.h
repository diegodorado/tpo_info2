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
#include "protocol.h"

void client_setup(void);

uint8_t client_rx_pop ( void);
void client_tx_push ( uint8_t);
uint8_t client_data_size (void);

uint8_t client_has_message();
message_hdr_t* client_get_message();




void client_send_status_response(message_hdr_t* request, status_id_t status);
void client_send_message_response(message_hdr_t* message, uint8_t* data);
void client_send_message(message_hdr_t* message, uint8_t* data);


#endif /* CLIENT_H_ */
