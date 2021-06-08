#include <list>
#include <deque>
#include <vector>
#include "Job.h"
#include "updateStatus.h"

void updateStatus(Job *&currentCpuJob, deque<Job *> *run_queue, list<Job *> &runningJobs_list, list<Job *> &sleepList, list<Job *> &ioWaitList, list<Job *> &endProcess, int currentCycle, int &timeInterval, Job *&nullJob)
{

    timeInterval++; //for lru-sampled
    if (currentCpuJob->pid != -1)
    {
        // for all Job add runningTime
        currentCpuJob->runningTime++;

        // check response time by real start time -starting time
        if (currentCpuJob->responseTime == -1)
        {
            currentCpuJob->responseTime = currentCycle - currentCpuJob->startCycle;
            currentCpuJob->realStartTime = currentCycle;
        }

        // count waiting time
        for (int k = 0; k < 10; k++)
        {
            if (!run_queue[k].empty())
            {
                for (int i = 0; i < run_queue[k].size(); i++)
                {
                    run_queue[k][i]->waitingTime++;
                    run_queue[k][i]->timeQuantum--;
                }
            }
        }
        //if currentCpuJob is done.
        if (currentCpuJob->cmdComplete)
        {
            endProcess.push_back(currentCpuJob);
            currentCpuJob = nullJob;
        }

        //if currentCpujob goes to sleeping
        if (currentCpuJob->sleepState)
        {
            currentCpuJob->sleepState = false;
            currentCpuJob = nullJob;
        }

        //if currentCpuJob goes to ioblock
        if (currentCpuJob->IOwaitState)
        {
            currentCpuJob->IOwaitState = false;
            currentCpuJob = nullJob;
        }
    }

    // if (currentCpuJob->cmdComplete)
    // {
    //     endProcess.push_back(currentCpuJob);
    //     currentCpuJob = nullJob;
    // }
    // else
    // {
    //     timeInterval++;
    // }
}