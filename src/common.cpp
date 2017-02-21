/*
 * common.cpp
 *
 *  Created on: 21.02.2017
 *      Author: cwiec
 */
#include "common.h"

void cpyAndSetCleanBuf(char* current, char* last,char* cleanBuf, size_t size)
{
	for(unsigned int i = 0; i < size; ++i)
	{
		if(current[i] == '\0')
		{
			cleanBuf[i] = '}';
			cleanBuf[i+1] = '\0';
			last[i] = current[i];
			break;
		}
		if(current[i] == last[i])
		{
		    cleanBuf[i] = ' ';
		}
		else
		{
			cleanBuf[i] = last[i];
		}
		last[i] = current[i];
	}
}

