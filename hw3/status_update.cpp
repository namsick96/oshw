#include "status_update.h"

void status_update(Job *&currentCpuJob, deque<Job *> *run_queue, int currentCycle)
{
    if (currentCpuJob->pid != -1)
    {
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
    }
}