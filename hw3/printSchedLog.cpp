#include "printSchedLog.h"

void printSchedLog(FILE *out, Job *&currentCpuJob, deque<Job *> *run_queue, list<Job *> &sleepList, list<Job *> &ioWaitList, int currentCycle)
{

    int pid;
    const char *name;
    int priority;
    int cycle = currentCycle;
    int opidx;
    int line;
    int op;
    int arg;
    list<Job *>::iterator job_iter;

    pid = currentCpuJob->pid;
    name = currentCpuJob->file_name.c_str();
    priority = currentCpuJob->priority;

    //printf("sched start\n");

    if (currentCpuJob->pid != -1)
    {
        opidx = currentCpuJob->current_cmd_int - 1;
        line = opidx + 1;
        op = currentCpuJob->cmd_list[opidx].first;
        arg = currentCpuJob->cmd_list[opidx].second;
    }

    //printf("oplineidxline\n");

    // line 1
    fprintf(out, "[%d Cycle] Scheduled Process: ", cycle);
    if (currentCpuJob->runningTime == 0)
    {
        fprintf(out, "%d %s (priority %d)\n", pid, name, priority);
    }
    else
    {
        fprintf(out, "None\n");
    }
    //printf("line1");
    // line 2
    fprintf(out, "Running Process: ");
    if (currentCpuJob->pid != -1)
    {
        fprintf(out, "Process#%d(%d) running code %s line %d(op %d, arg %d)\n", pid, priority, name, line, op, arg);
    }
    else
    {
        fprintf(out, "None\n");
    }
    //printf("line2");

    // line 3 runque
    for (int k = 0; k < 10; k++)
    {
        fprintf(out, "RunQueue %d: ", k);
        if (run_queue[k].empty())
        {
            fprintf(out, "Empty");
        }
        else
        {
            for (int i = 0; i < run_queue[k].size(); i++)
            {
                fprintf(out, "%d(%s) ", run_queue[k][i]->pid, run_queue[k][i]->file_name.c_str());
            }
        }
        fprintf(out, "\n");
    }

    // line 4
    fprintf(out, "SleepList: ");
    if (sleepList.empty())
    {
        fprintf(out, "Empty");
    }
    else
    {
        for (job_iter = sleepList.begin(); job_iter != sleepList.end(); job_iter++)
        {
            Job *job = *job_iter;
            fprintf(out, "%d(%s) ", job->pid, job->file_name.c_str());
        }
    }
    fprintf(out, "\n");

    // line 5
    fprintf(out, "IOWait List: ");
    if (ioWaitList.empty())
    {
        fprintf(out, "Empty");
    }
    else
    {
        for (job_iter = ioWaitList.begin(); job_iter != ioWaitList.end(); job_iter++)
        {
            Job *job = *job_iter;
            fprintf(out, "%d(%s) ", job->pid, job->file_name.c_str());
        }
    }
    fprintf(out, "\n");

    fprintf(out, "\n");
}