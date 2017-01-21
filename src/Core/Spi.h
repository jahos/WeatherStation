/*
 * Spi.h
 *
 *  Created on: 08.01.2017
 *      Author: cwiec
 */

#ifndef CORE_SPI_H_
#define CORE_SPI_H_

#include <queue>
#include <stdint.h>
#include "stm32f10x_gpio.h"

#define BUFFER_EMPTY	256

typedef std::queue<int> BufferQueue;

enum CommandE
{
		COMMAND = 0
	,	DATA	= 1
};

struct CommandS
{
	int msg;
	CommandE d_c;
};

enum SpiE
{
		eSPI1 = 0
	,	eSPI2 = 1
	, 	eSPI3 = 2
};

struct CSsetS
{
	GPIO_TypeDef* gpioType;
	uint16_t csPin;
};

class Spi {

public:
	/**
	 * @brief  check whether SPI is busy
	 * @param None
	 * @retval None
	 */
	virtual bool isBusy() = 0;

	/**
	 * @brief  get byte from out buffer
	 * @param None
	 * @retval byte to send through SPI
	 */
	virtual int getByte() = 0;

	/**
	 * @brief  store byte to in buffer
	 * @param byte to store
	 * @retval None
	 */
	virtual void storeByte(int byte) = 0;

	/**
	 * @brief  enable interrupt for TXE
	 * @param None
	 * @retval None
	 */
	virtual void send() = 0;

	/*destructor*/
	virtual ~Spi() {};
};

#endif /* CORE_SPI_H_ */
