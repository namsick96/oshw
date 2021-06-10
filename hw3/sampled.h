#include "PhysicalMemoryTree.h"
#include "Job.h"
#include <list>
#include <math.h>
#include <vector>
#include <deque>
#include <algorithm>
#ifndef __SAMPLED__
#define __SAMPLED__

void sampled(Job *&currentCpuJob, PhysicalMemoryTree &physicalMemory, deque<Job *> *run_queue, list<Job *> &sleepList, list<Job *> &ioWaitList);
bool cmpAllocID2(const pair<Job *, int> &a, const pair<Job *, int> &b);

#endif