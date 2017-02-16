/*
 * TaskMenager.cpp
 *
 *  Created on: 14.02.2017
 *      Author: cwiec
 */

#include <Core/TaskMenager.h>
#include <stdio.h>
#include <stm32f10x_gpio.h>

uint8_t TaskMenager::jobCount = 0;

void TaskMenager::run()
{
	if(!isEmpty())
	{
		Job jb = jobList[end++];
		GPIO_SetBits(GPIOC,GPIO_Pin_8);
		jb();
		GPIO_ResetBits(GPIOC,GPIO_Pin_8);
		end = (end % TASK_MENAGER_MAX_JOB_COUNT);
		jobCount--;
	}
}

bool TaskMenager::isEmpty()
{
	bool retVal;
	retVal = (jobCount > 0) ? false : true;
	return retVal;
}

void TaskMenager::addJob(Job newJob)
{
	if(jobCount < TASK_MENAGER_MAX_JOB_COUNT)
	{
		jobList[begin++] = newJob;
		begin = (begin % TASK_MENAGER_MAX_JOB_COUNT);
		jobCount++;
	}
	else
	{
		printf("Job queue full!\n\r");
	}
}

TaskMenager::TaskMenager():
		begin(0),
		end(0)
{
}

TaskMenager::~TaskMenager()
{
}

