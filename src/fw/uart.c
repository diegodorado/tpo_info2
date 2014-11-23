/*
 * uart.c
 *
 *  Created on: Sep 19, 2014
 *      Author: diego
 */


#include "uart.h"

//private static buffers, read and written only by function calls
static uart_buffer_t uart0_tx_buffer;
static uart_buffer_t uart0_rx_buffer;
static uart_buffer_t uart1_tx_buffer;
static uart_buffer_t uart1_rx_buffer;

//private functions
static void buffer_push ( uart_buffer_t * , uint8_t );
static uint8_t buffer_pop ( uart_buffer_t * );



void uart0_setup(void)
{

  //reset buffers indexes
  uart0_rx_buffer.in_index = 0;
  uart0_rx_buffer.out_index = 0;
  uart0_tx_buffer.in_index = 0;
  uart0_tx_buffer.out_index = 0;


  //1.- Registro PCONP (0x400FC0C4) - bit 3 en 1 prende la UART:
  PCONP |= 0x01<<3;
  //2.- Registro PCLKSEL0 (0x400FC1A8) - bits 6 y 7 en 0 seleccionan que el clk de la UART0 sea CCLK/4:
  PCLKSEL0 &= ~(0x03<<6); //con un CCLOK=100Mhz, nos queda PCLOCK=25Mhz
  //3.- Registro U1LCR (0x4001000C) - transmision de 8 bits, 1 bit de stop, sin paridad, sin break cond, DLAB = 1:
  UART0->LCR = 0x83;
    #if defined (BAUDRATE_2400)
    UART1->DLM = 0x02;   // es el resultado de 25Mhz/(2400*16)---> 651
    UART1->DLL = 0x8D;
    #elif defined (BAUDRATE_9600)
    //4.- Registros U1DLL (0x40010000) y U1DLM (0x40010004) - 9600 baudios:
    UART0->DLM = 0X00;
    UART0->DLL = 0xA3;//0xA3 para 9600
    #elif defined (BAUDRATE_19200)
    UART0->DLM = 0x00;   // es el resultado de 25Mhz/(38400*16)---> 41
    UART0->DLL = 0x00;
    #elif defined (BAUDRATE_38400)
    UART0->DLM = 0x00;   // es el resultado de 25Mhz/(38400*16)---> 41
    UART0->DLL = 0x28;
    #elif defined (BAUDRATE_115200)
    UART0->DLM = 0x00;   // es el resultado de 25Mhz/(115200*16)---> 13.6 -- 14
    UART0->DLL = 0x0D;
    #endif

  //5.- Registros PINSEL0 (0x4002C000) y PINSEL1 (0x4002C004) - habilitan las funciones especiales de los pines:
  //5. habilitan las funciones especiales de los pines:
  set_pin_sel(U0TX_PIN,1);
  set_pin_sel(U0RX_PIN,1);
  //6.- Registro U1LCR, pongo DLAB en 0:
  UART0->LCR &= ~(0x01<<7);// pongo en cero el bit 7 DLAB=0
  //7. Habilito las interrupciones (En la UART -IER- y en el NVIC -ISER)
  UART0->IER |= 0X03 ; // bit 0 y 1 del registro U1Ier Habilia int por TX y RX
  ISER0 |= (1<<5);
}




void uart1_setup(void)
{

  //reset buffers indexes
  uart1_rx_buffer.in_index = 0;
  uart1_rx_buffer.out_index = 0;
  uart1_tx_buffer.in_index = 0;
  uart1_tx_buffer.out_index = 0;

  //TODO: Completar Inicializacion de UART1
  //1.- Registro PCONP: Energizo la UART:
  //1.- Registro PCONP (0x400FC0C4) - bit 4 en 1 prende la UART:
  PCONP |= 0x10;

  //2.- Registro PCLKSEL0 (0x400FC1A8) - selecciono el clk de la UART1
  // bits 8 y 9 en 0 seleccionan que el clk de la UART1 sea 25MHz--> DIVIDE POR 4
  PCLKSEL0 &= ~(0x03<<8);

  //3.- Registro U1LCR (0x4001000C)
  // transmision de 8 bits, 1 bit de stop, sin paridad, sin break cond, DLAB = 1
  UART1->LCR =0x83;

  //4.- Registros U1DLL (0x40010000) y U1DLM (0x40010004) en 0xA2:
  #if defined (BAUDRATE_2400)
  UART1->DLM = 0x02;   // es el resultado de 25Mhz/(2400*16)---> 651
  UART1->DLL = 0x8D;
  #elif defined (BAUDRATE_9600)
  UART1->DLM = 0x00;   // es el resultado de 25Mhz/(9600*16)---> 163
  UART1->DLL = 0xA3;
  #elif defined (BAUDRATE_19200)
  UART1->DLM = 0x00;   // es el resultado de 25Mhz/(9600*16)---> 163
  UART1->DLL = 0xA3;
  #elif defined (BAUDRATE_38400)
  UART1->DLM = 0x00;   // es el resultado de 25Mhz/(38400*16)---> 41
  UART1->DLL = 0x28;
  #elif defined (BAUDRATE_115200)
  UART1->DLM = 0x00;   // es el resultado de 25Mhz/(115200*16)---> 13.6 -- 14
  UART1->DLL = 0x0D;
  #endif

  //5. habilitan las funciones especiales de los pines:
  set_pin_sel(U1TX_PIN,1);
  set_pin_sel(U1RX_PIN,1);

  //6. Habilito las interrupciones (En la UART -IER- y en el NVIC -ISER)
  // hay que poner el DLAB=0 para habilitar las interrupciones por TX RX;
  UART1->LCR &= ~(0x01<<7);// pongo en cero el bit 7 DLAB=0
  UART1->IER |= 0X07 ; // bit 0 y 1 y 2 del registro U1Ier Habilia int por TX y RX
  ISER0 |= (0x01<< 6);//  Habilita Interrupcion por UART1 del NVIC  Pag77


}







void uart0_rx_push ( uint8_t data )
{
  buffer_push(&uart0_rx_buffer, data);
}


uint8_t uart0_rx_pop ( void)
{
  //todo: check error of no data
  return buffer_pop(&uart0_rx_buffer);

}

void uart0_tx_push ( uint8_t data )
{
  buffer_push(&uart0_tx_buffer, data);
  //Si esta vacio el THR
  if ( UART0->LSR.THRE)
    UART0->THR = uart0_tx_pop();

}


uint8_t uart0_tx_pop ( void)
{
  //todo: check error of no data
  return buffer_pop(&uart0_tx_buffer);

}





void uart1_rx_push ( uint8_t data )
{
  buffer_push(&uart1_rx_buffer, data);
}


uint8_t uart1_rx_pop ( void)
{
  //todo: check error of no data
  return buffer_pop(&uart1_rx_buffer);

}


void uart1_tx_push ( uint8_t data )
{
  buffer_push(&uart1_tx_buffer, data);
  //Si esta vacio el THR
  if ( UART1->LSR.THRE )
    UART1->THR = uart1_tx_pop();

}


uint8_t uart1_tx_pop ( void)
{
  //todo: check error of no data
  return buffer_pop(&uart1_tx_buffer);

}

uint8_t uart0_rx_data_size (void) { return BUFFER_DATA_SIZE(uart0_rx_buffer); }
uint8_t uart0_tx_data_size (void) { return BUFFER_DATA_SIZE(uart0_tx_buffer); }
uint8_t uart1_rx_data_size (void) { return BUFFER_DATA_SIZE(uart1_rx_buffer); }
uint8_t uart1_tx_data_size (void) { return BUFFER_DATA_SIZE(uart1_tx_buffer); }


static void buffer_push ( uart_buffer_t * buf_ptr, uint8_t data )
{
  buf_ptr->data[buf_ptr->in_index] = data;
  buf_ptr->in_index++;
  buf_ptr->in_index %= BUFFER_SIZE(buf_ptr->data);
}

static uint8_t buffer_pop ( uart_buffer_t * buf_ptr)
{
  //todo: handle no more data
  uint8_t result = 0;

  if(buf_ptr->out_index!=buf_ptr->in_index){
    //only read buffer if out_index != in_index
    result = buf_ptr->data[buf_ptr->out_index];
    buf_ptr->out_index++;
    buf_ptr->out_index %= BUFFER_SIZE(buf_ptr->data);
  }

  return result;
}
