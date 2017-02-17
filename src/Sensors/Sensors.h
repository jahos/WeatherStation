/*
 * Sensors.h
 *
 *  Created on: 20.01.2017
 *      Author: cwiec
 */

#ifndef SENSORS_SENSORS_H_
#define SENSORS_SENSORS_H_

#include <queue>
#include <Core/Spi.h>

template <typename T>
T abs(T var)
{
	return ((var > 0)? var : (-1 * var));
}

struct MeasureData
{
	uint16_t color;
	char * data;
};

typedef enum SensorsE
{
	HIH6030e  = 0,
	MiCS6814e = 1,
	BMP280e	 = 2
}SensorsE;

class Sensors
{
private:

public:
	virtual void drawMe() = 0;
	virtual void sendMeasureReq() = 0;
	virtual ~Sensors() {}
};

#endif /* SENSORS_SENSORS_H_ */
