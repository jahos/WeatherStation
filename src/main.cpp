/*
******************************************************************************
File:     main.cpp
Info:     Generated by Atollic TrueSTUDIO(R) 7.0.1   2016-12-27

The MIT License (MIT)
Copyright (c) 2009-2016 Atollic AB

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
******************************************************************************
*/

/* Includes */

#include <stddef.h>
#include <stdio.h>
#include "stm32f10x.h"
#include "STM32vldiscovery.h"
#include "userSettings.h"
#include "Sensors/HIH6030.h"
#include "Display/Display.h"

/* Private macro */
/* Private variables */

/* Private function prototypes */
/* Private functions */

/**
**===========================================================================
**
**  Abstract: main program
**
**===========================================================================
*/
template <typename T>
T abs(T var)
{
	return ((var > 0)? var : (-1 * var));
}

Display ds;

void pomiar()
{
	static HIH6030 humSens;
	static int i = 0;
	if(i%2)
	{
		humSens.measureRequest();
	}
	else
	{
		humSens.getMeasurements();

		printf("temperatura[%i.%i *C]\n\r",(humSens.getTemperature()/100),abs(humSens.getTemperature()%100));
		printf("Wilgotnosc[%i.%i %c]\n\r",(humSens.getHumidity()/100),(humSens.getHumidity()%100),'%');
		i=0;
	}
	++i;
}

extern void (*wsk2)();


int main(void)
{
	wsk2 = pomiar;
	init();
	printf("Witaj !\n\r");
//	ds.setBackground(BLACK);
//	ds.drawPixel(10,10,RGB_MACRO(255,0,0));
	ds.setBackground(BLUE);
	while (1)
	{

	}
}

/*
 * Minimal __assert_func used by the assert() macro
 * */
extern "C" void __assert_func(const char *file, int line, const char *func, const char *failedexpr)
{
  while(1)
  {}
}

/*
 * Minimal __assert() uses __assert__func()
 * */
extern "C" void __assert(const char *file, int line, const char *failedexpr)
{
   __assert_func (file, line, NULL, failedexpr);
}


