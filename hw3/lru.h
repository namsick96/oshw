#include <string>
#include <cstring>
#include <list>
#include "Job.h"
#include "PhysicalMemoryTree.h"
#ifndef __LRU__
#define __LRU__

void lru(Job *&currentCpuJob, PhysicalMemoryTree &physicalMemory);

#endif //PRACTICE1_PAGECHANGE_LRU_H