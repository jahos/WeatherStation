/*
 * MiCS6814.h
 *
 *  Created on: 14.02.2017
 *      Author: cwiec
 */

#ifndef SENSORS_MICS6814_H_
#define SENSORS_MICS6814_H_
#include <stdint.h>
#include "Sensors.h"

class MiCS_6814 {
private:
	static uint16_t adcVal[4];
	float sensorCO;
	float sensorNO2;
	float sensorNH3;
	float sensorC3H8;
	float sensorC4H10;
	float sensorCH4;
	float sensorH2;
	float sensorC2H5OH;
	static const int R0_sensCO;
	static const int R0_sensNO2;
	static const int R0_sensNH3;
	float RS_sensCO;
	float RS_sensNO2;
	float RS_sensNH3;
	char stringBuf[10];

	float getRS(uint16_t adcVal);
	void convertVal(float val, const char* pref);
public:
	static uint16_t* getBuffer();
	void makeMeasure();
	int getRefVal();
	char* get_sensCO();
	char* get_sensNO2();
	char* get_sensNH3();
	char* get_sensC3H8();
	char* get_sensC4H10();
	char* get_sensCH4();
	char* get_sensH2();
	char* get_sensC2H5OH();
	MiCS_6814();
	~MiCS_6814();
};

#endif /* SENSORS_MICS6814_H_ */
