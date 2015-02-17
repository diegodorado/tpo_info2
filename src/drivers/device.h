/*
 * micro.h
 *
 *  Created on: Sep 19, 2014
 *      Author: diego
 */

#ifndef DEVICE_H_
#define DEVICE_H_

#define DEVICE_RGB_RED_PIN 2,3
#define DEVICE_RGB_GREEN_PIN 2,2
#define DEVICE_RGB_BLUE_PIN 2,1

//include firmware
#include "fw.h"

typedef  enum{
  DEVICE_RGB_NONE,
  DEVICE_RGB_RED,
  DEVICE_RGB_GREEN,
  DEVICE_RGB_BLUE,
} device_rgb_color_t;

void device_setup(void);
void device_rgb_set(device_rgb_color_t color);


#endif /* DEVICE_H_ */
