/*
 * DisplayMenager.cpp
 *
 *  Created on: 17.02.2017
 *      Author: cwiec
 */

#include <Display/DisplayMenager.h>

DisplayMenager::DisplayMenager() : currentDispSens(HIH6030e)
{

}

DisplayMenager::~DisplayMenager()
{

}


void DisplayMenager::setSensor(SensorsE sensor)
{
	currentDispSens = sensor;
}

void DisplayMenager::showMeasurements(Sensors* sensor)
{
	if(sensor != 0)
	{
		sensor->drawMe();
	}
}

void DisplayMenager::draw()
{
	switch(currentDispSens)
	{
	case HIH6030e:
	{
		showMeasurements(humSens);
		break;
	}
	case MiCS6814e:
	{
//		showMeasurements(gasSens);
		break;
	}
	case BMP280e:
	{
		break;
	}
	default:
		display->display_string(0,12,"Not found!",FONT_1206,RED);
		break;
	}
}
