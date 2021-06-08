#include "PhysicalMemoryTree.h"
#include "Job.h"
#include <list>
#include <math.h>
#include <vector>
#include <deque>
#ifndef __CMDMEMFREE__
#define __CMDMEMFREE__

void cmd_memFree(Job *&currentCpuJob, PhysicalMemoryTree &physicalMemory, int opparam);

#endif