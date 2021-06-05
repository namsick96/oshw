#include "commandExecute.h"

void commandExecute(Job *&currentCpuJob, list<Job *> &sleepList, list<Job *> &ioWaitList, list<int> &lockList, list<Job *> &runningJobs_list, int currentCycle) //, list<PageBundle *> *all_pages, PageBundle **physicalMem, string sched_opt, string page_opt, int &aid, int physicalNum, int currentCycle, int &page_fault);
{
    if (currentCpuJob->pid != -1)
    {
        vector<pair<int, int> > cmdVec = currentCpuJob->cmd_list;
        int cmdVecidx = currentCpuJob->current_cmd_int;

        int op = cmdVec[cmdVecidx].first;
        int opparam = cmdVec[cmdVecidx].second;

        //execute

        currentCpuJob->current_cmd_int++;
        if (currentCpuJob->current_cmd_int++ == currentCpuJob->cmd_list.size())
        {
            currentCpuJob->cmdComplete = true;
        }
    }
}