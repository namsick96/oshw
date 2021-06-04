#include <vector>
#include <string>
#include <string.h>
#include <queue>
#include "Job.h"

#ifndef __SCHEDULER__
#define __SCHEDULER__

class scheduler
{
public:
    queue<Job *> *run_queue;

    scheduler(queue<Job *> *runQueue)
    {
        run_queue = runQueue;
    }

    Job *scheduling(Job *currentCpuJob);
};

#endif //__SCHEDULER__
