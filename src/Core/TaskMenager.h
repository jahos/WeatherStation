/*
 * TaskMenager.h
 *
 *  Created on: 14.02.2017
 *      Author: cwiec
 */

#ifndef CORE_TASKMENAGER_H_
#define CORE_TASKMENAGER_H_
#include <stdint.h>

#define TASK_MENAGER_MAX_JOB_COUNT 20

typedef void (*Job)();

class TaskMenager {
private:
	static uint8_t jobCount;
	int begin;
	int end;
	Job jobList[TASK_MENAGER_MAX_JOB_COUNT];
	uint8_t getCapacity();
public:
	uint8_t getJobCount() {return begin;}
	bool isEmpty();
	void run();
	void addJob(Job jb);
	TaskMenager();
	~TaskMenager();
};

#endif /* CORE_TASKMENAGER_H_ */
