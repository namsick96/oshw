#include "PhysicalMemoryTree.h"
#include "Job.h"
#include "lru.h"
#include <list>
#include <math.h>
#include <vector>
#include <deque>
#include "sampled.h"
#include "second_chance.h"
#ifndef __CMDMEMACCESS__
#define __CMDMEMACCESS__

void cmd_memAccess(Job *&currentCpuJob, PhysicalMemoryTree &physicalMemory, int opparam, int currentCycle, string page, int &page_fault, deque<Job *> *run_queue, list<Job *> &sleepList, list<Job *> &ioWaitList);

#endif