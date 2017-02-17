/*
 * HIH6030.cpp
 *
 *  Created on: 20.01.2017
 *      Author:
 */

#include <Sensors/HIH6030.h>
#include <userSettings.h>
#include <stdio.h>
#include <string.h>

HIH6030::HIH6030():
	 m_temperature(0)
	,m_humidity(0)
	,m_sp(&SPI2_class::getInstance())
{
	memset(m_hum,'\0',10);
	memset(m_temp,'\0',10);
	temp.color = 30;
	temp.data = &m_temp[0];
	hum.color = 20;
	hum.data = &m_hum[0];
	m_csSetting.gpioType = GPIOB;
	m_csSetting.csPin = GPIO_Pin_12;
	m_sp->setBuffers(&m_inBuf,&m_outBuf);
	m_sp->setCS(m_csSetting);
}

HIH6030::~HIH6030()
{

}

MeasureData* HIH6030::getTemperature()
{
	return &temp;
}

MeasureData* HIH6030::getHumidity()
{
	return &hum;
}

void HIH6030::sendMeasureReq()
{
	if(m_sp->isBusy() ==  false)
	{
		m_outBuf.push(0xAA);
		m_sp->send();
	}
	m_inBuf.pop();
}

void HIH6030::updatedColors()
{
	if(m_humidity > 60)
	{
		hum.color = 16;
	}
	else
	{
		hum.color = 32 - ((int)m_humidity >> 2);
	}

	if(m_temperature < 0)
	{
		temp.color = 0;
	}
	else if(m_temperature > 30)
	{
		temp.color = 32;
	}
	else
	{
		temp.color = (int)m_temperature + 1;
	}
}

void HIH6030::updateBuffers()
{
	//floating points
	int temp_m =(int)(abs(m_temperature*100))%100;
	int hum_m = (int)(m_humidity*100)%100;

	if(temp_m < 10)
	{
		sprintf(m_temp,"%i.0%i",(int)m_temperature,temp_m);
	}
	else
	{
		sprintf(m_temp,"%i.%i",(int)m_temperature,temp_m);
	}
	if(hum_m < 10)
	{
		sprintf(m_hum,"%i.0%i",(int)m_humidity,hum_m);
	}
	else
	{
		sprintf(m_hum,"%i.%i",(int)m_humidity,hum_m);
	}
}

void HIH6030::collectData()
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

	updatedColors();
	updateBuffers();
}

void HIH6030::drawMe()
{
	humSens->collectData();
	ds->display_string(30,12,"~C",FONT_1206,colorScale[temp.color]);
	ds->display_string(30,0,"\%",FONT_1206,colorScale[hum.color]);
	ds->display_string(0,0,hum.data,FONT_1206,colorScale[hum.color]);
	ds->display_string(0,12,temp.data,FONT_1206,colorScale[temp.color]);
}
