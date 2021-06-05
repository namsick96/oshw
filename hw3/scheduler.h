#include <vector>
#include <string>
#include <string.h>
#include <queue>
#include <deque>
#include "Job.h"

#ifndef __SCHEDULER__
#define __SCHEDULER__

class scheduler
{
public:
    deque<Job *> *run_queue;
    Job *nullJob;

    scheduler(deque<Job *> *runQueue, Job *null_Job)
    {
        run_queue = runQueue;
        nullJob = null_Job;
    }

    Job *scheduling(Job *currentCpuJob);
};

#endif //__SCHEDULER__
