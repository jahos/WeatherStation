/*
 * BMP280.h
 *
 *  Created on: 20.01.2017
 *      Author:
 */

#ifndef SENSORS_BMP280_H_
#define SENSORS_BMP280_H_

#include "Core/SPI2class.h"

class BMP280
{
private:
	int preasure;
	int temperature;
	SPI2_class * sp;
public:
	BMP280();
	virtual ~BMP280();
};

#endif /* SENSORS_BMP280_H_ */
