/*
 * SPI2class.cpp
 *
 *  Created on: 19.01.2017
 *      Author: cwiec
 */

#include <Core/SPI2class.h>
#include "userSettings.h"
#include <stdio.h>
#include <queue>



static int (*pGetByte)() = 0;
static void (*pStoreByte)(int byte) = 0;
static void (*pDisableCS)() = 0;

static void storeByte(int byte)
{
	SPI2_class & sp = SPI2_class::getInstance();
	sp.storeByte(byte);
}

static int getByte()
{
	SPI2_class & sp = SPI2_class::getInstance();
	int retVal = sp.getByte();
	return retVal;
}

static void disableCS()
{
	SPI2_class & sp = SPI2_class::getInstance();
	if(sp.isLastByte())
	{
		sp.disableCS();
	}
}

SPI2_class& SPI2_class::getInstance()
{
	static SPI2_class sp;
	return sp;
}

SPI2_class::SPI2_class()
{
	m_inBuffer = 0;
	m_outBuffer = 0;
	pStoreByte = ::storeByte;
	pGetByte = ::getByte;
	pDisableCS = ::disableCS;
}

void SPI2_class::setBuffers(BufferQueue* inBuf, BufferQueue* outBuf)
{
	m_inBuffer = inBuf;
	m_outBuffer = outBuf;
}

void SPI2_class::setCS(CSsetS settings)
{
	m_csSet.gpioType = settings.gpioType;
	m_csSet.csPin = settings.csPin;
}

void SPI2_class::disableCS()
{
	GPIO_SetBits(m_csSet.gpioType,m_csSet.csPin);
}

bool SPI2_class::isBusy()
{
	bool state = SPI_I2S_GetITStatus(SPI2,SPI_I2S_IT_TXE);
	return state;
}

int SPI2_class::getByte()
{
	int byteToSend = BUFFER_EMPTY;
	if(m_outBuffer != 0)
	{
		if(m_outBuffer->size() != 0 )
		{
			byteToSend = m_outBuffer->front();
			GPIO_ResetBits(m_csSet.gpioType,m_csSet.csPin);
			m_outBuffer->pop();
		}
	}
	else
	{
		printf("m_outBuffer NULL\n\r");
	}
	return byteToSend;
}

void SPI2_class::storeByte(int byte)
{
	if(m_inBuffer != 0)
	{
		m_inBuffer->push(byte);
	}
	else
	{
		printf("m_inBuffer NULL\n\r");
	}
}

void SPI2_class::send()
{
	SPI_I2S_ITConfig(SPI2,SPI_I2S_IT_TXE,ENABLE);
	SPI_I2S_ITConfig(SPI2,SPI_I2S_IT_RXNE,ENABLE);
}

bool SPI2_class::isLastByte()
{
	bool retVal = false;
	if(m_outBuffer->size() < 1)
	{
		retVal =  true;
	}
	return retVal;
}

SPI2_class::~SPI2_class()
{

}

extern "C" void SPI2_IRQHandler()
{
	pDisableCS();

	if(SPI_I2S_GetITStatus(SPI2,SPI_I2S_IT_TXE) != RESET)
	{
		int c = pGetByte();
		if(c <= 0xFF)
		{
			SPI_I2S_SendData(SPI2,(uint8_t)c);
		}
		else
		{
			 SPI_I2S_ITConfig(SPI2, SPI_I2S_IT_TXE, DISABLE);
		}
	}

	if (SPI_I2S_GetITStatus(SPI2, SPI_I2S_IT_RXNE) != RESET)
	{
		int tmp = SPI_I2S_ReceiveData(SPI2);
		pStoreByte(tmp);
		SPI_I2S_ClearFlag(SPI2,SPI_I2S_FLAG_RXNE);
	}
}



