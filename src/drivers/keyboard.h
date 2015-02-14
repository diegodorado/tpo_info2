/*
 * keyboard.h
 *
 *  Created on: Sep 19, 2014
 *      Author: diego
 */

#ifndef KEYBOARD_H_
#define KEYBOARD_H_

//include firmware
#include "fw.h"



#define     EINT0_PIN       2,10
#define     EINT1_PIN       2,11
#define     EINT2_PIN       2,12
#define     EINT3_PIN       2,13

#define     KEY0_PIN       2,10
#define     KEY1_PIN       0,18
#define     KEY2_PIN       0,11
#define     KEY3_PIN       2,13

#define KEY_PIN(i) KEY##i##_PIN

void keyboard_setup(void);
void keyboard_set_handler(void (*handler)( uint8_t key));

#endif /* KEYBOARD_H_ */
