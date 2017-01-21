/*
 * HIH6030.cpp
 *
 *  Created on: 20.01.2017
 *      Author:
 */

#include <Sensors/HIH6030.h>
#include <stdio.h>

HIH6030::HIH6030():
	 m_temperature(0)
	,m_humidity(0)
	,m_sp(&SPI2_class::getInstance())
{
	m_csSetting.gpioType = GPIOB;
	m_csSetting.csPin = GPIO_Pin_12;
	m_sp->setBuffers(&m_inBuf,&m_outBuf);
	m_sp->setCS(m_csSetting);
}

HIH6030::~HIH6030()
{

}

int HIH6030::getTemperature()
{
	return (m_temperature * 100);
}

int HIH6030::getHumidity()
{
	return (m_humidity * 100);
}

void HIH6030::measureRequest()
{
	if(m_sp->isBusy() ==  false)
	{
		m_outBuf.push(0xAA);
		m_sp->send();
	}
	m_inBuf.pop();
}

void HIH6030::getMeasurements()
{
	if(m_sp->isBusy() ==  false)
	{
		m_outBuf.push(0xFF);
		m_outBuf.push(0xFF);
		m_outBuf.push(0xFF);
		m_outBuf.push(0xFF);
		m_sp->send();
	}

	int tmp = 0;
	unsigned int pomiar = 0;
	do
	{
		tmp = m_inBuf.front();
		m_inBuf.pop();
		pomiar = (pomiar << 8);
		pomiar |= tmp;

	}while(m_inBuf.size());

	int h = (pomiar >> 16 ) & 0x3FFF;
	int t = (pomiar >> 2 ) & 0x3FFF;

	m_humidity 	= (((float)h * 100) / 16382);
	m_temperature = (((float)t * 165) / 16382)-40;
}

