/*
 * DisplayMenager.h
 *
 *  Created on: 17.02.2017
 *      Author: cwiec
 */

#ifndef DISPLAY_DISPLAYMENAGER_H_
#define DISPLAY_DISPLAYMENAGER_H_
#include "userSettings.h"
#include "Sensors/Sensors.h"

class DisplayMenager {
private:
	SensorsE currentDispSens;
	void showMeasurements(Sensors *s);
public:
	void setSensor(SensorsE sensor);
	void draw();
	DisplayMenager();
	~DisplayMenager();
};

#endif /* DISPLAY_DISPLAYMENAGER_H_ */
