/*
 * HIH6030.h
 *
 *  Created on: 20.01.2017
 *      Author:
 */

#ifndef SENSORS_HIH6030_H_
#define SENSORS_HIH6030_H_

#include "Core/SPI2class.h"

class HIH6030
{
private:
	float m_temperature;
	float m_humidity;
	std::queue<int> m_outBuf;
	std::queue<int> m_inBuf;
	CSsetS m_csSetting;
	SPI2_class * m_sp;
public:
	void measureRequest();
	void getMeasurements();
	int getTemperature();
	int getHumidity();
	HIH6030();
	virtual ~HIH6030();
};

#endif /* SENSORS_HIH6030_H_ */
