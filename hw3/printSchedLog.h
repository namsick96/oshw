#include <list>
#include <vector>
#include <deque>
#include <stdio.h>
#include "Job.h"

#ifndef __PRINTSCHEDLOG__
#define __PRINTSCHEDLOG__

void printSchedLog(FILE *out, Job *&currentCpuJob, deque<Job *> *run_queue, list<Job *> &sleepList, list<Job *> &ioWaitList, int currentCycle);

#endif
