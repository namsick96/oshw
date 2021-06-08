#include <string>
#include <list>
#include "Job.h"
#include "PhysicalMemoryTree.h"
#ifndef __LRU__
#define __LRU__

int lru(Job *&currentCpuJob, PhysicalMemoryTree &physicalMemory);

#endif //PRACTICE1_PAGECHANGE_LRU_H