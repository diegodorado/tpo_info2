#include "cmd_sd.h"
#include "spi.h"
//#include "lpc_1769.h"

#define TRUE 1
#define FALSE 0

#define TFE     0x01
#define TNF     0x02
#define RNE     0x04
#define RFF     0x08
#define BSY     0x10

// SP0SPCR  Bit-Definitions
#define CPHA    3
#define CPOL    4
#define MSTR    5

// SP0SPSR  Bit-Definitions
#define SPIF	7

#define SPI_IODIR		LPC_GPIO0->FIODIR
#define SPI_SCK_PIN		7		
#define SPI_SSEL_PIN		6		
#define SPI_MISO_PIN		8		
#define SPI_MOSI_PIN		9		

#define SPI_PINSEL			LPC_PINCON->PINSEL0
#define SPI_SCK_FUNCMASK	(3 << 14) // P0.7- PINSEL0 [15:14]
#define SPI_SSEL_FUNCMASK	(3 << 12)  // P0.6 - PINSEL0 [13:12]
#define SPI_MISO_FUNCMASK	(3 << 16)  // P0.8 - PINSEL0 [17:16]
#define SPI_MOSI_FUNCMASK	(3 << 18)  // P0.9 - PINSEL0 [19:18]

#define SPI_SCK_FUNCBIT		(2<< 14)
#define SPI_SSEL_FUNCBIT	(0 << 12) // Note - SSEL needed as GPIO
#define SPI_MISO_FUNCBIT	(2 << 16)
#define SPI_MOSI_FUNCBIT	(2 << 18)

#define SPI_PRESCALE_REG	LPC_SPI->SPCCR

#define SPI_PRESCALE_MIN  8

#define SELECT_CARD()   LPC_GPIO0->FIOCLR = (1 << SPI_SSEL_PIN)
#define UNSELECT_CARD() LPC_GPIO0->FIOSET = (1 << SPI_SSEL_PIN)

volatile uint32_t j;

void spi_init(void){
	/*
	LPC_SC->PCONP |= (1<<8); // PCSPI

		// Clock

		//LPC_SC->PCLKSEL0 &= ~(3<<16);  // PCLK_SPI
		//LPC_SC->PCLKSEL0 |=  (1<<16);  // PCLK_periph = CCLK


		LPC_SC->PCLKSEL0 &= ~(3<<20);	// Select PCLK frequency for SSP
			LPC_SC->PCLKSEL0 |= (1<<20);

		// setup GPIO
		SPI_IODIR |= (1 << SPI_SCK_PIN) | (1 << SPI_MOSI_PIN) | (1 << SPI_SSEL_PIN);
		SPI_IODIR &= ~(1 << SPI_MISO_PIN);

		//// reset Pin-Functions
		// P0.15 set to SCK
		LPC_PINCON->PINSEL0 &= ~SPI_SCK_FUNCMASK;
		LPC_PINCON->PINSEL0 |=  SPI_SCK_FUNCBIT;
		// P0.16, P0.17, P0.18 set to SSEL,MISI, MOSI
		SPI_PINSEL &= ~(SPI_MOSI_FUNCMASK | SPI_MISO_FUNCMASK | SPI_SSEL_FUNCMASK);
		SPI_PINSEL |=  (SPI_MOSI_FUNCBIT | SPI_MISO_FUNCBIT | SPI_SSEL_FUNCBIT);

		//LPC_SPI->CR0 = 0x0007;				//Set mode: SPI mode 0, 8-bit
			//SSP1CR1 = 0x2;					//Enable SSP with Master

		// enable SPI-Master
		LPC_SPI->SPCR = (1 << MSTR) | (0 << CPOL);	// TODO: check CPOL

		UNSELECT_CARD();

		// Code for GPIO setup
	    // Switch the SSI TX line to a GPIO and drive it high too.

		LPC_PINCON->PINSEL0 &= ~(SPI_MOSI_FUNCMASK);
		LPC_GPIO0->FIODIR |= (1<<SPI_MOSI_PIN);
		LPC_GPIO0->FIOSET = (1<<SPI_MOSI_PIN);

		// low speed during init
		if_spiSetSpeed(254);

		// Send 20 spi commands with card not selected
		for(i=0;i<21;i++)
			my_if_spiSend(iface,0xff);

		LPC_PINCON->PINSEL0 |= (SPI_MOSI_FUNCBIT);
*/
}

uint8_t spi_read (uint8_t address){
	//spi_CS_LOW();
		//spi_CS_HIGH();
	return address;
}


uint8_t spi_write (uint8_t address){
return ERROR;
}

void spi_disable(void){
	j++;
return ;
}

void spi_wait(uint16_t wait){
j++;
}

uint8_t spi_response(uint8_t resp){
return resp;
}

void spi_cmd(uint8_t cmd){
cmd=0;
return ;
}

void spi_send_byte(uint8_t send_byte){
send_byte=0;
return ;
}

void spi_send_block(uint8_t block){
block=0;
return ;
}

uint8_t  spi_get(uint8_t mmc_get){
return mmc_get;
}





void spi_config_speed (uint8_t on) {

  if (on == TRUE) {
    //Max. 12 MBit used for Data Transfer.
   // LPC_SSP0->CPSR = 10;                      // 100MHz / 10 = 10MBit
  }
  else {
    // Max. 400 kBit used in Card Initialization.
  //  LPC_SSP0->CPSR = 250;                     // 100MHz / 250 = 400kBit
  }
  return ;
}


void spi_cs (uint32_t ss) {

  if (ss) {
   // LPC_GPIO0->FIOPIN |= (1<<16);
  }
  else {
//    LPC_GPIO0->FIOPIN &= ~(1<<16);
  }
}

uint8_t spi_send (uint8_t out) {

  //LPC_SSP0->DR = out;
  //while (LPC_SSP0->SR & BSY);                 // Wait for transfer to finish
//  return (LPC_SSP0->DR);                      //Return received value
	return ERROR;
}

