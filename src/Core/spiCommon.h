/*
 * spiCommon.h
 *
 *  Created on: 20.01.2017
 *      Author:
 */

#ifndef CORE_SPICOMMON_H_
#define CORE_SPICOMMON_H_
#include "stm32f10x_gpio.h"

bool setPointers(int (*a_pGetByte)()
			   ,void (*a_pStoreByte)(int byte)
			   ,void (*a_pEnableCS)());

#endif /* CORE_SPICOMMON_H_ */
