/*
 * SPI1class.cpp
 *
 *  Created on: 18.01.2017
 *      Author: cwiec
 */

#include <Core/SPI1class.h>
#include "spiCommon.h"
#include  "userSettings.h"
#include <stdio.h>
#include <queue>

static int (*pGetByteSpi)() = 0;
static void (*pStoreByteSpi)(int byte) = 0;
static void (*pDisableCSSpi)() = 0;

static void storeBytev(int byte)
{
	SPI1_class & sp = SPI1_class::getInstance();
	sp.storeByte(byte);
}

static int getBytev()
{
	SPI1_class & sp = SPI1_class::getInstance();
	int retVal = sp.getByte();
	return retVal;
}

static void disableCSv()
{
	SPI1_class & sp = SPI1_class::getInstance();
	if(sp.isLastByte())
	{
		sp.disableCS();
	}
}

SPI1_class& SPI1_class::getInstance()
{
	static SPI1_class sp;
	return sp;
}

SPI1_class::SPI1_class()
{
	pStoreByteSpi = storeBytev;
	pGetByteSpi = getBytev;
	pDisableCSSpi = disableCSv;
}

void SPI1_class::setCS(CSsetS settings)
{
	m_csSet.gpioType = settings.gpioType;
	m_csSet.csPin = settings.csPin;
}

void SPI1_class::disableCS()
{
	GPIO_SetBits(m_csSet.gpioType,m_csSet.csPin);
}

bool SPI1_class::isBusy()
{
	bool state = SPI_I2S_GetITStatus(SPI1,SPI_I2S_IT_TXE);
	return state;
}

int SPI1_class::getByte()
{
	int msg = BUFFER_EMPTY;

	if(m_outBuffer.size() != 0)
	{
		CommandS command = m_outBuffer.front();
		msg = command.msg;

		//update D/C# line
		GPIO_WriteBit(GPIOA,SPI1_D_C_PIN, (BitAction)command.d_c);
		GPIO_ResetBits(m_csSet.gpioType,m_csSet.csPin);
		//remove job from queue
		m_outBuffer.pop();
	}
	return msg;
}

void SPI1_class::storeByte(int byte)
{
}

void SPI1_class::storeCommand(int msg, CommandE cmd)
{
	CommandS commands;
	commands.msg = msg;
	commands.d_c = cmd;
	m_outBuffer.push(commands);
}

void SPI1_class::send()
{
	SPI_I2S_ITConfig(SPI1,SPI_I2S_IT_TXE,ENABLE);
}

bool SPI1_class::isLastByte()
{
	bool retVal = false;
	if(m_outBuffer.size() < 1)
	{
		retVal =  true;
	}
	return retVal;
}

SPI1_class::~SPI1_class()
{

}

extern "C" void SPI1_IRQHandler()
{
	pDisableCSSpi();

	if(SPI_I2S_GetITStatus(SPI1,SPI_I2S_IT_TXE) != RESET)
	{
		int c = pGetByteSpi();
		if(c <= 0xFF)
		{
			SPI_I2S_SendData(SPI1,(uint8_t)c);
		}
		else
		{
			 SPI_I2S_ITConfig(SPI1, SPI_I2S_IT_TXE, DISABLE);
		}
	}

	if (SPI_I2S_GetITStatus(SPI1, SPI_I2S_IT_RXNE) != RESET)
	{
		SPI_I2S_ClearFlag(SPI1,SPI_I2S_FLAG_RXNE);
	}
}


