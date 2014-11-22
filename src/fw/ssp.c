//todo: poner los puertos y pines a usar en defines del header
#include "ssp.h"


void ssp_setup(void)
{

  set_pin_sel(0,9,2);  // MOSI
  set_pin_sel(0,8,2);  // MISO
  set_pin_sel(0,7,2);  // SCK
  set_pin_sel(0,6,0);  // SSEL
  gpio_set_dir(0,6,1); // SSEL como GPIO (salida)

  PCLKSEL0 |= 1<<20;  //PCLK=CLCK

  SSP1->CR0 = 0x07;  //8bit, modo SPI, CPOL=0, CPHA=0
  SSP1->CPSR = 250;  //SPI a 400KHz
  SSP1->CR1 = 2;  //SSP1 encendido, modo master
}

void ssp_ssel_set_low()
{
  gpio_set_pin(0, 6, 0);
}

void ssp_ssel_set_high()
{
  gpio_set_pin(0, 6, 1);
}


void ssp_set_clock(uint32_t clk)
{
  SSP1->CPSR = 100000000/clk;
}

void ssp_set_clock_slow()
{
  ssp_set_clock(400000);  // Slow clock (100kHz-400kHz)
}

void ssp_set_clock_fast()
{
  ssp_set_clock(25000000);  // Fast clock (25MHz)
}



void ssp_send_data(uint16_t dato)
{
  uint16_t aux;

  while(SSP1->SR&4)
    aux = SSP1->DR;

  SSP1->DR = dato;

  for(aux=0;aux<500;aux++);

  while(SSP1->SR&4)
    aux = SSP1->DR;

}

uint16_t ssp_get_data(void)
{
  uint16_t aux;

  while(SSP1->SR&4)
  {
    aux = SSP1->DR;
  }
  //SSP1ICR = 0x03;
  SSP1->DR = 0xFF;

  for(aux=0;aux<500;aux++);

  while(!(SSP1->SR&4));
    return SSP1->DR;
}
