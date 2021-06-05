#include "status_update.h"

void status_update(Job *&currentCpuTask, deque<Job *> *run_queue, int currentCycle)
{
    if (currentCpuTask->pid != -1)
    {
        // check response time by real start time -starting time
        if (currentCpuTask->responseTime == -1)
        {
            currentCpuTask->responseTime = currentCycle - currentCpuTask->startCycle;
            currentCpuTask->realStartTime = currentCycle;
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