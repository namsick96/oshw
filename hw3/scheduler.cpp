#include "scheduler.h"

Job *scheduler::scheduling(Job *currentCpuJob)
{

    Job *job;
    int currentPriority = currentCpuJob->priority;
    int readyQueueTopPriority = 10;

    for (int i = 0; i < 10; i++)
    {
        if (!run_queue[i].empty())
        {
            readyQueueTopPriority = i;
            break;
        }
    }
    //No Scheduling item
    //in this case nullJob preceed
    if (readyQueueTopPriority == 10)
    {
        return currentCpuJob;
    }
    //initiating new process at ready_queue
    if (currentPriority == -1)
    {
        job = run_queue[readyQueueTopPriority].front();
        run_queue[readyQueueTopPriority].pop();
        return job;
    }

    //universial policy
    if (currentPriority > readyQueueTopPriority)
    {
        //new job
        job = run_queue[readyQueueTopPriority].front();
        run_queue[readyQueueTopPriority].pop();
        //initialize timeQuantum
        currentCpuJob->timeQuantum = 10;
        run_queue[currentPriority].push(currentCpuJob);
        return job;
    }
    else
    {
        //fcfs
        if (currentPriority <= 4)
        {
            return currentCpuJob;
        }
        //rr
        else
        {
            int timeQ = currentCpuJob->timeQuantum;
            if (timeQ == 0)
            {
                //new job
                job = run_queue[readyQueueTopPriority].front();
                run_queue[readyQueueTopPriority].pop();
                //initialize timeQuantum
                currentCpuJob->timeQuantum = 10;
                run_queue[currentPriority].push(currentCpuJob);
                return job;
            }
            else
            {
                return currentCpuJob;
            }
        }
    }
}