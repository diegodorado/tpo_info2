#include "periph.h"


/********************************************************************************
	\fn  void SetPINSEL (uint8_t puerto , uint8_t pin, uint8_t sel)
	\brief Selección de modo de los puertos (4 modos).
	\author & date: Informatica II
 	\param [in] puerto: port a configurar
 	\param [in] pin:	pin del port a configurar
 	\param [in] funcion:	selección de la funcion que tendra el pin  [0 - 3]
	\return void
*/
void set_pin_sel( uint8_t puerto , uint8_t pin ,uint8_t funcion )
{
	puerto = puerto * 2 + pin / 16;
	pin = ( pin % 16 ) * 2;
	PINSEL[ puerto ] = PINSEL[ puerto ] & ( ~ ( 3 << pin ) );
	PINSEL[ puerto ] = PINSEL[ puerto ] | ( funcion << pin );
}

/********************************************************************************
	\fn  void set_pin_mode( uint8_t port , uint8_t pin ,uint8_t modo)
	\brief Selección de modo de los puertos cuando trabajan como entradas
	\author & date: Informatica II
 	\param [in] puerto: port a configurar
 	\param [in] pin:	pin del port a configurar
 	\param [in] funcion:	selección del modo que tendra el pin  [0 - 3]
	\return void
*/
void set_pin_mode( uint8_t port , uint8_t pin ,uint8_t modo)
{
	port = port * 2 + pin / 16;
	pin = ( pin % 16 ) * 2;
	PINMODE[ port ] = PINMODE[ port ] & ( ~ ( 3 << pin ) );
	PINMODE[ port ] = PINMODE[ port ] | ( modo << pin );
}



/********************************************************************************
 \fn  void void SetDIR( uint8_t puerto , uint8_t pin , uint8_t dir )
 \brief Establece si un determinado PIN de un determinado PUERTO (previamente
   configurado como GPIO) es entrada o salida.
 \author & date: Informatica II
  \param [in] puerto:  puerto con el que se va a trabajar
  \param [in] pin:  pin a configurar sentido
  \param [in] dir:  Direccion 0 = entrada - 1 = salida.
 \return void
*/
void gpio_set_dir( uint8_t puerto , uint8_t pin , uint8_t dir )
{
 puerto = puerto * 8;

 GPIO[ puerto ] = GPIO[ puerto ] & ( ~ ( 1 << pin ) );
 GPIO[ puerto ] = GPIO[ puerto ] | ( dir << pin );
}

/********************************************************************************
 \fn  void SetPIN( uint8_t puerto , uint8_t pin , uint8_t estado )
 \brief Establece un ESTADO en un determinado PIN de un determinado PUERTO.
 \author & date: Informatica II
  \param [in] puerto: puerto con el que se va a trabajar
  \param [in] pin: pin a activar estado
  \param [in] estado: valor a establecer en el PIN del PUERTO [0-1].
 \return void
*/
void gpio_set_pin( uint8_t puerto , uint8_t pin , uint8_t estado )
{
 puerto = puerto * 8 + 5;

 if(estado)
  GPIO[ puerto ] |=   1 << pin;
 else
  GPIO[ puerto ] &=  ~ ( 1 << pin );
}

/********************************************************************************
 \fn  uint8_t GetPIN( uint8_t puerto , uint8_t pin , uint8_t actividad )
 \brief Devuelve el ESTADO de un determinado PIN de un determinado PUERTO.
 \author & date: Informatica II
  \param [in] puerto: puerto con el que se va a trabajar
  \param [in] pin: pin a consultar estado
 \param [in] actividad: ALTO = 1, BAJO = 0
 \return: estado del pin consultado (uint_8)
*/
uint8_t gpio_get_pin( uint8_t puerto , uint8_t pin , uint8_t actividad )
{
 puerto = puerto * 8 + 5;

 return ( ( ( GPIO[ puerto ] >> pin ) & 1 ) == actividad ) ? 1 : 0;

}

