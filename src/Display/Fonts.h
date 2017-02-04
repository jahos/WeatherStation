/*
 * Fonts.h
 *
 *  Created on: Feb 24, 2016
 *      Author: Jan
 */

#ifndef FONTS_H_
#define FONTS_H_
#include "stm32f1xx_it.h"
#include <stdint.h>

#define FONT_1206    12
#define FONT_1608    16

extern const uint8_t c_chFont1206[95][12];
extern const uint8_t c_chFont1608[95][16];
extern const uint8_t c_chFont1612[11][32];
extern const uint8_t c_chFont3216[11][64];
extern const uint8_t c_chBmp4016[96];
extern const uint8_t c_chSingal816[16];
extern const uint8_t c_chMsg816[16];
extern const uint8_t c_chBluetooth88[8];
extern const uint8_t c_chBat816[16];
extern const uint8_t c_chGPRS88[8];
extern const uint8_t c_chAlarm88[8];

#endif /* FONTS_H_ */
