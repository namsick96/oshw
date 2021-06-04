#include "check_ioWait.h"

void check_ioWait(queue<Job *> *run_queue, list<Job *> &ioWaitList, vector<InputIO *> &input_list, int currentCycle)
{
    list<Job *>::iterator job_iter;
    //list<InputIO *>::iterator io_iter;

    //list<InputIO *> tmpIO;
    list<Job *> tempJob;

    int input_list_leftpopNum = 0;
    for (int k = 0; k < input_list.size(); k++)
    {
        if (input_list[k]->startCycle == currentCycle)
        {
            input_list_leftpopNum++;
            // find task fitted pid
            for (job_iter = ioWaitList.begin(); job_iter != ioWaitList.end(); job_iter++)
            {
                Job *job = *job_iter;

                if (job->pid == input_list[k]->target_pid)
                {
                    if (job->memoryCursor != 10)
                    {
                        tempJob.push_back(job); // append for delete check
                        job->timeQuantum = 10;
                        run_queue[job->priority].push(job); // append for run, input done
                    }
                    else
                    { //what is this? check when memory case
                        tempJob.push_back(job);
                        job->timeQuantum = 10;
                    }
                }
            }
        }
    }

    // remove input_list that has already inputted
    input_list.erase(input_list.begin(), input_list.begin() + input_list_leftpopNum);

    // remove task which receive IO
    for (job_iter = tempJob.begin(); job_iter != tempJob.end(); job_iter++)
    {
        Job *job = *job_iter;
        ioWaitList.remove(job);
    }
}
