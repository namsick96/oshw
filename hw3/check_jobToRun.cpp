#include "check_jobToRun.h"

void check_jobToRun(deque<Job *> *run_queue, list<Job *> &runningJobs_list, vector<Job *> &jobs_list, int currentCycle)
{
    int jobs_list_eraseNum = 0;

    for (int k = 0; k < jobs_list.size(); k++)
    {
        Job *job = jobs_list[k];
        if (job->startCycle == currentCycle)
        {
            jobs_list_eraseNum++;
            job->timeQuantum = 10;
            run_queue[job->priority].push_back(job);
        }
    }

    // remove input_list that has already inputted
    jobs_list.erase(jobs_list.begin(), jobs_list.begin() + jobs_list_eraseNum);
}