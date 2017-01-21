/*
 * SPI1class.h
 *
 *  Created on: 18.01.2017
 *      Author: cwiec
 */

#ifndef CORE_SPI1CLASS_H_
#define CORE_SPI1CLASS_H_

#include "Spi.h"

class SPI1_class : public Spi
{
protected:
	/*contructor*/
	SPI1_class();
	/*pointers to input buffer and output buffer*/


	/*settings of chip select which have to be load to SPI class
	 * depends of sensor
	 * */
	volatile CSsetS m_csSet;

public:
	std::queue<CommandS> m_outBuffer;
	/**
	 * @brief  load chip select settings to SPI class
	 * @param  chip select settings
	 * @retval None
	 */
	void setCS(CSsetS settings);

	/**
	 * @brief  turn of chip select
	 * @param  None
	 * @retval None
	 */
	void disableCS();

	/*implementation of member function from SPI class*/
	bool isBusy();

	int getByte();

	void storeByte(int byte);

	void storeCommand(int msg, CommandE cmd = COMMAND);

	void send();

	/**
	 * @brief  check whether it is the last byte to send
	 * @param  None
	 * @retval true if it is the last one
	 */
	bool isLastByte();

	/**
	 * @brief  get instance of SPI2_class
	 * @param  None
	 * @retval Instance to SPI2 class
	 */
	static SPI1_class & getInstance();

	virtual ~SPI1_class();
};

#endif /* CORE_SPI1CLASS_H_ */
