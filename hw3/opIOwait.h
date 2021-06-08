#include <list>
#include <string>
#include <string.h>
#include "Job.h"

#ifndef __OPIOWAIT__
#define __OPIOWAIT__

void opIOwait(Job *&currentCpuJob, list<Job *> &ioWaitList);

#endif