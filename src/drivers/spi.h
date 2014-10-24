#include "sysdefs.h"
#ifdef USE_XTAL_OSC
#define	CCLK		12000000UL	/* cclk frequency [Hz] */
#define PCLK_SSP	12000000UL	/* PCLK frequency for SSP [Hz] */
#define SCLK_FAST	 6000000UL	/* SCLK frequency on work [Hz] */
#define	SCLK_SLOW	  400000UL	/* SCLK frequency on initialization [Hz] */
#else
#define	CCLK		4000000UL	/* cclk frequency [Hz] */
#define PCLK_SSP	4000000UL	/* PCLK frequency for SSP [Hz] */
#define SCLK_FAST   2000000UL	/* SCLK frequency on work [Hz] */
#define	SCLK_SLOW	 400000UL	/* SCLK frequency on initialization [Hz] */
#endif

#define SPI_CLOCKRATE_LOW   (uint32_t ) (250)      		/* 100MHz / 250 = 400kHz */
#define SPI_CLOCKRATE_HIGH  (uint32_t ) (10)     		/* 100MHz / 10 = 10MHz   */

#define	spi_CS_LOW()	LPC_GPIO0->FIOCLR = 1<<6	/* Set P0.6 low */
#define	spi_CS_HIGH() LPC_GPIO0->FIOSET = 1<<6	/* Set P0.6 high */

#define SPI1 1
#define SPI0 0
#define BUFFER_MAX 512
#define  MODE_8BIT 8UL
#define RESPONSE_OK 1
#define ERROR 	-1
uint32_t  	sector;

static void spi_init(void);
static void spi_config_speed (uint8_t on);
static void spi_disable(void);
static void spi_wait(uint16_t wait);
static void spi_cmd(uint8_t cmd);
static void spi_send_byte(uint8_t);
static void spi_send_block(uint8_t);

uint8_t spi_read (uint8_t address);
uint8_t spi_write (uint8_t address);
uint8_t spi_response(uint8_t resp);
uint8_t  spi_get(uint8_t);

