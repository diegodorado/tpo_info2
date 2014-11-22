//todo: implementar con interrupciones y resolver los timeouts que no estan siendo efectivos ahora

#include "fw.h"
#include "sd_card.h" //redundant include...

volatile uint32_t sdtimer1, sdtimer2;

static uint8_t CardType; /* flags indicadores de tipo de memoria */
extern volatile uint32_t sdtimer1, sdtimer2; //fixme: nadie actualiza estos valores


/*-----------------------------------------------------------------------*/
/* Deselecciona la tarjeta y libera el bus SPI                           */
/*-----------------------------------------------------------------------*/
static void sd_card_deassert(void)
{
  ssp_ssel_set_high();
  ssp_get_data();
}

/*-----------------------------------------------------------------------*/
/* Seleccciona la memoria y espera a que este lista                      */
/*-----------------------------------------------------------------------*/

static uint32_t sd_card_assert(void) /* TRUE:Successful, FALSE:Timeout */
{
  uint16_t res;
  ssp_ssel_set_low();
  sdtimer2 = 50;
  res = ssp_get_data();
  while ((res != 0xFF) && sdtimer2)
    res = ssp_get_data();
  if ( res != 0xFF )
  {
    sd_card_deassert();
    return 0;
  }
  else
    return 1;
}

static uint8_t sd_card_ready(void)
{
  uint8_t res;
  sdtimer2 = 50; /* Wait for ready in timeout of 500ms */
  res = ssp_get_data();
  while ((res != 0xFF) && sdtimer2)
  {
    res = ssp_get_data();
  }

  return res;
}

static uint8_t sd_card_send_cmd(uint8_t cmd, uint32_t arg)
{
  uint8_t n, res;

  if ( cmd & 0x80 )
  { /* ACMD<n> is the command sequense of CMD55-CMD<n> */
    cmd &= 0x7F;
    res = sd_card_send_cmd(CMD55, 0);
    if ( res > 1 )
      return res;
  }

  /* Select the card and wait for ready */
  sd_card_deassert();
  if ( !sd_card_assert() )
    return 0xFF;

  /* Envío paquete completo */
  ssp_send_data(cmd); /* Start + Comando */
  ssp_send_data((uint8_t) (arg >> 24)); /* Argumento[31..24] */
  ssp_send_data((uint8_t) (arg >> 16)); /* Argumento[23..16] */
  ssp_send_data((uint8_t) (arg >> 8)); /* Argumento[15..8] */
  ssp_send_data((uint8_t) arg); /* Argumento[7..0] */
  n = 0x01; /* CRC + Stop */
  if ( cmd == CMD0)
    n = 0x95; /* CRC valido para CMD0(0) */
  if ( cmd == CMD8)
    n = 0x87; /* CRC valido para CMD8(0x1AA) */
  ssp_send_data(n); /* Unicos CRC necesarios */

  /* Receive command response */
  if ( cmd == CMD12)
    ssp_get_data(); /* Se desecha un dato luego de CMD12 (detener transmision) */

  for (n = 0, res = 0xFF; (n < 10) && (res & 0x80); n++) /* Se espera una respuesta valida en un maximo de 10 intentos */
    res = ssp_get_data();

  return res; /* Se devuelve la respuesta */
}

static uint8_t sd_card_get_block(uint8_t *buf, uint32_t size)
{
  uint8_t token = 0xFF;
  uint32_t i = 0;

  sdtimer1 = 20;
  while (token == 0xFF && sdtimer1) /* Wait for data packet in timeout of 200ms */
  {
    token = ssp_get_data();
  }
  if ( token != 0xFE )
    return 0; /* If not valid data token, return with error */

  while (size)
  {
    buf[i] = ssp_get_data();
    size--;
    i++;
  }
  ssp_get_data(); /* Discard CRC */
  ssp_get_data();

  return 1; /* Return with success */
}

static uint8_t sd_card_send_block(uint8_t *buf, uint32_t size, uint8_t token)
{
  uint32_t i = 0, res;

  while (!sd_card_ready())
    ;

  ssp_send_data(token);
  if ( token != 0xFD ) /* Is data token */
  {
    while (size)
    {
      ssp_send_data(buf[i]);
      size--;
      i++;
    }
    ssp_send_data(0xFF); /* Dummy CRC */
    ssp_send_data(0xFF);
    res = ssp_get_data(); /* Receive data response */
    if ( (res & 0x1F) != 0x05 ) /* If not accepted, return with error */
      return 0;
  }
  return 1; /* Return with success */
}

uint32_t sd_card_setup(void)
{
  uint8_t n, cmd, type, ocr[4];

  ssp_setup();
  ssp_set_clock_slow();
  for (n = 0; n < 10; n++)
    ssp_get_data(); /* 80 clocks */

  type = 0;
  if ( sd_card_send_cmd(CMD0, 0) == 1 )
  { /* Enter Idle state */
    sdtimer1 = 100; /* Initialization timeout of 1000 msec */
    if ( sd_card_send_cmd(CMD8, 0x1AA) == 1 )
    { /* SDHC */
      for (n = 0; n < 4; n++)
        ocr[n] = ssp_get_data(); /* Get trailing return value of R7 resp */
      if ( ocr[2] == 0x01 && ocr[3] == 0xAA )
      { /* The card can work at vdd range of 2.7-3.6V */
        while (sdtimer1 && sd_card_send_cmd(ACMD41, 1 << 30))
          ; /* Wait for leaving idle state (ACMD41 with HCS bit) */
        if ( sdtimer1 && sd_card_send_cmd(CMD58, 0) == 0 )
        { /* Check CCS bit in the OCR */
          for (n = 0; n < 4; n++)
            ocr[n] = ssp_get_data();
          type = (ocr[0] & 0x40) ? CT_SD2 | CT_BLOCK : CT_SD2; /* SDv2 */
        }
      }
    }
    else
    { /* SDSC or MMC */
      if ( sd_card_send_cmd(ACMD41, 0) <= 1 )
      {
        type = CT_SD1;
        cmd = ACMD41; /* SDv1 */
      }
      else
      {
        type = CT_MMC;
        cmd = CMD1; /* MMCv3 */
      }
      while (sdtimer1 && sd_card_send_cmd(cmd, 0))
        ; /* Wait for leaving idle state */
      if ( !sdtimer1 || sd_card_send_cmd(CMD16, 512) != 0 ) /* Set R/W block length to 512 */
        type = 0;
    }
  }
  CardType = type;
  sd_card_deassert();

  if ( type )
  { /* Initialization succeded */
    ssp_set_clock_fast();
    return 1;
  }
  else
    /* Initialization failed */
    return 0;
}

uint8_t sd_card_read(uint8_t* buf, uint32_t size, uint32_t block)
{
  uint8_t res = 1;
  if ( !(CardType & CT_BLOCK) )
    block *= 512; /* Convert to byte address if needed */

  if ( size == 1 )
  { /* Single block read */
    if ( (sd_card_send_cmd(CMD17, block) == 0) && sd_card_get_block(buf, 512) ) /* READ_SINGLE_BLOCK */
      size = 0;
    else
      res = 0;
  }
  else
  { /* Multiple block read */
    if ( sd_card_send_cmd(CMD18, block) == 0 )
    { /* READ_MULTIPLE_BLOCK */
      while (size)
      {
        if ( !sd_card_get_block(buf, 512) )
        {
          res = 0;
          break;
        }
        buf += 512;
        size--;
      }
      sd_card_send_cmd(CMD12, 0); /* STOP_TRANSMISSION */
    }
    else
      res = 0;
  }
  sd_card_deassert();
  return res;
}

uint8_t sd_card_write(uint8_t* buf, uint32_t size, uint32_t block)
{
  uint8_t res = 1;

  if ( !(CardType & CT_BLOCK) )
    block *= 512; /* Convert to byte address if needed */

  if ( size == 1 )
  { /* Single block write */
    if ( (sd_card_send_cmd(CMD24, block) == 0)
        && sd_card_send_block(buf, 512, 0xFE) ) /* WRITE_BLOCK */
      size = 0;
    else
      res = 0;
  }
  else
  { /* Multiple block read */
    if ( CardType & CT_SDC)
      sd_card_send_cmd(ACMD23, /*block*/size);
    if ( sd_card_send_cmd(CMD25, block) == 0 ) /* WRITE_MULTIPLE_BLOCK */
    {
      while (size)
      {
        if ( !sd_card_send_block(buf, 512, 0xFC) )
        {
          res = 0;
          break;
        }
        buf += 512;
        size--;
      }
      if ( !sd_card_send_block(NULL, 0, 0xFD) ) /* STOP_TRAN token */
        res = 0;
    }
    else
      res = 0;
  }
  sd_card_deassert();
  return res;
}
