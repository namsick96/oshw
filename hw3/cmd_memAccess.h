#include "PhysicalMemoryTree.h"
#include "Job.h"
#include "lru.h"
#include <list>
#include <math.h>
#include <vector>
#include <deque>
#ifndef __CMDMEMACCESS__
#define __CMDMEMACCESS__

void cmd_memAccess(Job *&currentCpuJob, PhysicalMemoryTree &physicalMemory, int opparam, int currentCycle, string page, int &page_fault);

#endif