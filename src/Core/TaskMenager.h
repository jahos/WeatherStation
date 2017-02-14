/*
 * TaskMenager.h
 *
 *  Created on: 14.02.2017
 *      Author: cwiec
 */

#ifndef CORE_TASKMENAGER_H_
#define CORE_TASKMENAGER_H_
#include <queue>

typedef void (*Job)();

class TaskMenager {
private:
	std::queue<Job> jobList;
public:
	void run();
	void addJob(Job jb);
	TaskMenager();
	~TaskMenager();
};

#endif /* CORE_TASKMENAGER_H_ */
