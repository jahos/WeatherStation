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

class Sensors
{
private:
	std::queue<Spi*> jobQ;
public:
	void addJob(Spi* sp);
	void makeMeasure();
	Sensors();
	~Sensors();
};

#endif /* SENSORS_SENSORS_H_ */
