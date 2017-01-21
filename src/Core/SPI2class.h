/*
 * SPI2class.h
 *
 *  Created on: 19.01.2017
 *      Author: cwiec
 */

#ifndef CORE_SPI2CLASS_H_
#define CORE_SPI2CLASS_H_

#include "Spi.h"

class SPI2_class : public Spi
{
protected:
	/*contructor*/
	SPI2_class();
	/*pointers to input buffer and output buffer*/
	BufferQueue* m_outBuffer;
	BufferQueue* m_inBuffer;

	/*settings of chip select which have to be load to SPI class
	 * depends of sensor
	 * */
	volatile CSsetS m_csSet;
public:

	/**
	 * @brief  load setting to SPI class
	 * @param  input buffer and output buffer
	 * @retval None
	 */
	void setBuffers(BufferQueue* inBuf,BufferQueue* outBuf);

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
	static SPI2_class & getInstance();

	virtual ~SPI2_class();
};

#endif /* CORE_SPI2CLASS_H_ */
