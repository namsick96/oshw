#include <list>
#include <string>
#include <string.h>
#include "Job.h"

#ifndef __OPSLEEP__
#define __OPSLEEP__

void opSleep(Job *&currentCpuJob, int opParam, list<Job *> &sleepList);

#endif