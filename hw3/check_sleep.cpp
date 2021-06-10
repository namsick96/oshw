#include "check_sleep.h"

void check_sleep(deque<Job *> *run_queue, list<Job *> &sleepList)
{
    list<Job *>::iterator job_iter;
    list<Job *> temp;

    for (job_iter = sleepList.begin(); job_iter != sleepList.end(); job_iter++)
    {
        Job *job = *job_iter;
        job->sleepTime--;
        if (job->sleepTime == 0)
        {
            temp.push_back(job);
            int jobPriority = job->priority;
            job->timeQuantum = 10;
            job->runningTime = 0;
            run_queue[jobPriority].push_back(job); // go back to runqueue
        }
    }

    for (job_iter = temp.begin(); job_iter != temp.end(); job_iter++)
    {
        Job *job = *job_iter;
        sleepList.remove(job);
    }
};