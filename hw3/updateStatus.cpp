#include <list>
#include <deque>
#include <vector>
#include "Job.h"
#include "updateStatus.h"

void updateStatus(Job *&currentCpuJob, deque<Job *> *run_queue, list<Job *> &runningJobs_list, list<Job *> &sleepList, list<Job *> &ioWaitList, list<Job *> &endProcess, int currentCycle, int &timeInterval, Job *&nullJob)
{

    if (currentCpuJob->pid != -1)
    {
        // for all Job add runningTime
        currentCpuJob->runningTime++;
    }

    if (currentCpuJob->cmdComplete)
    {
        endProcess.push_back(currentCpuJob);
        currentCpuJob = nullJob;
    }
    else
    {
        timeInterval++;
    }
}