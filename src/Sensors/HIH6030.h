/*
 * HIH6030.h
 *
 *  Created on: 20.01.2017
 *      Author:
 */

#ifndef SENSORS_HIH6030_H_
#define SENSORS_HIH6030_H_

#include "Core/SPI2class.h"
#include "Sensors.h"

class HIH6030 : public Sensors
{
private:
	float m_temperature;
	float m_humidity;
	std::queue<int> m_outBuf;
	std::queue<int> m_inBuf;
	CSsetS m_csSetting;
	SPI2_class * m_sp;
	char m_hum[10];
	char m_temp[10];
	char m_lastHum[10];
	char m_lastTemp[10];
	char m_cleanBuf[10];
	MeasureData temp;
	MeasureData hum;
	void updatedColors();
	void updateBuffers();
public:
	bool cleanFlag;
	void drawMe();
	void sendMeasureReq();
	void collectData();
	MeasureData* getTemperature();
	MeasureData* getHumidity();
	HIH6030();
	virtual ~HIH6030();
};

#endif /* SENSORS_HIH6030_H_ */
