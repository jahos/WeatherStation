/*
 * Sensors.cpp
 *
 *  Created on: 20.01.2017
 *      Author: cwiec
 */

#include <Sensors/Sensors.h>

Sensors::Sensors()
{

}

Sensors::~Sensors()
{

}

void Sensors::makeMeasure()
{
	while(jobQ.size() != 0)
	{
		Spi* sp = jobQ.front();
//		sp->makeMeasure();
		jobQ.pop();
	}
}

void Sensors::addJob(Spi* sp)
{
	jobQ.push(sp);
}

