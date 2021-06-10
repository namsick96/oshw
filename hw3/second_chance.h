#include "PhysicalMemoryTree.h"
#include "Job.h"
#include <list>
#include <math.h>
#include <vector>
#include <deque>
#include <algorithm>
#ifndef __SECONDCHANCE__
#define __SECONDCHANCE__

bool cmpAllocID(const pair<Job *, int> &a, const pair<Job *, int> &b);
void second_chance(Job *&currentCpuJob, PhysicalMemoryTree &physicalMemory, deque<Job *> *run_queue, list<Job *> &sleepList, list<Job *> &ioWaitList);

#endif