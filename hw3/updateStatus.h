#include <list>
#include <deque>
#include <vector>
#include "Job.h"
#include "cmd_memFree.h"
#include "PhysicalMemoryTree.h"

#ifndef __UPDATESTATUS__
#define __UPDATESTATUS__

void updateStatus(Job *&currentCpuJob, deque<Job *> *run_queue, list<Job *> &runningJobs_list, list<Job *> &sleepList, list<Job *> &ioWaitList, list<Job *> &endProcess, int currentCycle, int &timeInterval, Job *&nullJob, PhysicalMemoryTree &physicalMemory, string page);

#endif
