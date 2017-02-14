/*
 * TaskMenager.cpp
 *
 *  Created on: 14.02.2017
 *      Author: cwiec
 */

#include <Core/TaskMenager.h>

void TaskMenager::run()
{
	if(!jobList.empty())
	{
		Job jb = jobList.front();
		jobList.pop();
		jb();
	}
}

void TaskMenager::addJob(Job jb)
{
	jobList.push(jb);
}

TaskMenager::TaskMenager()
{
}

TaskMenager::~TaskMenager()
{
}

