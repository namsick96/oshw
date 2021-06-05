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
    //Null task go
    //in this case nullJob preceed
    if (readyQueueTopPriority == 10 && currentPriority == -1)
    {
        return currentCpuJob;
    }
    //initiating new process at ready_queue
    //Nulljob iter finish, new job ready. change to newjob
    if (readyQueueTopPriority != 10 && currentPriority == -1)
    {
        job = run_queue[readyQueueTopPriority].front();
        job->runningTime = 0;
        run_queue[readyQueueTopPriority].pop_front();
        return job;
    }

    //universial policy
    if (currentPriority > readyQueueTopPriority)
    {
        //new job
        job = run_queue[readyQueueTopPriority].front();
        job->runningTime = 0;
        run_queue[readyQueueTopPriority].pop_front();

        //initialize timeQuantum and go back to ready queue
        currentCpuJob->timeQuantum = 10;
        run_queue[currentPriority].push_back(currentCpuJob);
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
                if (readyQueueTopPriority != 10)
                {
                    job = run_queue[readyQueueTopPriority].front();
                    run_queue[readyQueueTopPriority].pop_front();
                    job->runningTime = 0;
                    //initialize timeQuantum
                    currentCpuJob->timeQuantum = 10;
                    run_queue[currentPriority].push_back(currentCpuJob);
                    return job;
                }
                else
                {
                    currentCpuJob->timeQuantum = 10;
                    run_queue[currentPriority].push_back(currentCpuJob);
                    currentCpuJob->runningTime = 0;
                    return nullJob;
                }
            }
            else
            {
                return currentCpuJob;
            }
        }
    }
}