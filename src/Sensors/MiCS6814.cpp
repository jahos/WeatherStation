/*
 * MiCS6814.cpp
 *
 *  Created on: 14.02.2017
 *      Author: cwiec
 */

#include <Sensors/MiCS6814.h>
#include <math.h>
#include <string.h>
#include <stdio.h>

uint16_t MiCS_6814::adcVal[4] = {0};
const int MiCS_6814::R0_sensCO = 100; //152
const int MiCS_6814::R0_sensNH3 = 100;
const int MiCS_6814::R0_sensNO2 = 1;

MiCS_6814::MiCS_6814() :
				sensorCO(0),
				sensorC3H8(0),
				sensorC4H10(0),
				sensorCH4(0),
				sensorH2(0),
				sensorC2H5OH(0),
				RS_sensCO(0),
				RS_sensNO2(0),
				RS_sensNH3(0)
{
	memset(stringBuf,'\0',10);
}

MiCS_6814::~MiCS_6814()
{
}

uint16_t* MiCS_6814::getBuffer()
{
	return &adcVal[0];
}

int MiCS_6814::getRefVal()
{
	return adcVal[0];
}

void MiCS_6814::convertVal(float val, const char * pref)
{
	int tmp =(int)(abs(val*100))%100;

	if(tmp < 10)
	{
		sprintf(stringBuf,"%s%i.0%ippm",pref,(int)val,tmp);
	}
	else
	{
		sprintf(stringBuf,"%s%i.%ippm",pref,(int)val,tmp);
	}
}

char* MiCS_6814::get_sensCO()
{
	convertVal(sensorCO,"CO:");
	return &stringBuf[0];
}

char* MiCS_6814::get_sensNO2()
{
	convertVal(sensorNO2,"NO2:");
	return &stringBuf[0];
}

char* MiCS_6814::get_sensNH3()
{
	convertVal(sensorNH3,"NH3:");
	return &stringBuf[0];
}

char* MiCS_6814::get_sensC3H8()
{
	convertVal(sensorC3H8,"C3H8:");
	return &stringBuf[0];
}

char* MiCS_6814::get_sensC4H10()
{
	convertVal(sensorC4H10,"C4H10:");
	return &stringBuf[0];
}

char* MiCS_6814::get_sensCH4()
{
	convertVal(sensorCH4,"C4H:");
	return &stringBuf[0];
}

char* MiCS_6814::get_sensH2()
{
	convertVal(sensorH2,"H2:");
	return &stringBuf[0];
}

char* MiCS_6814::get_sensC2H5OH()
{
	convertVal(sensorC2H5OH,"C2H5OH:");
	return &stringBuf[0];
}

float MiCS_6814::getRS(uint16_t adcValue)
{
	float retVal = 0;
	float VCC = 4915.2 / adcVal[3];
	float V0 = (adcValue * VCC) / 4096;
	retVal = (V0 * 46.2)/(VCC - V0);
	return retVal;
}

void MiCS_6814::makeMeasure()
{
	float ratio0, ratio1, ratio2;

	RS_sensCO 	= getRS(adcVal[0]);
	RS_sensNH3 	= getRS(adcVal[1]);
	RS_sensNO2 	= getRS(adcVal[2]);

	ratio0 = RS_sensNH3 / R0_sensNH3;
	ratio1 = RS_sensCO / R0_sensCO;
	ratio2 = RS_sensNO2 / R0_sensNO2;

	sensorCO 	= pow(ratio1, -1.179)*4.385;
	sensorNO2 	= pow(ratio2, 1.007)/6.855;
	sensorNH3 	= pow(ratio0, -1.67)/1.47;
	sensorC3H8 	= pow(ratio0, -2.518)*570.164;
	sensorC4H10 = pow(ratio0, -2.138)*398.107;
	sensorCH4	= pow(ratio1, -4.363)*630.957;
	sensorH2	= pow(ratio1, -1.8)*0.73;
	sensorC2H5OH = pow(ratio1, -1.552)*1.622;
}

