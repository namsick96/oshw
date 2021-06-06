#include "commandExecute.h"

void commandExecute(Job *&currentCpuJob, list<Job *> &sleepList, list<Job *> &ioWaitList, list<int> &lockList, list<Job *> &runningJobs_list, int currentCycle) //, list<PageBundle *> *all_pages, PageBundle **physicalMem, string sched_opt, string page_opt, int &aid, int physicalNum, int currentCycle, int &page_fault);
{
    cout << "start" << endl;
    if (currentCpuJob->pid != -1)
    {

        //vector<pair<int, int> > cmdVec = currentCpuJob->cmd_list;
        //int cmdVecidx = currentCpuJob->current_cmd_int;
        cout << "assign" << endl;

        int op = currentCpuJob->cmd_list[currentCpuJob->current_cmd_int].first;       //cmdVec[cmdVecidx].first;
        int opparam = currentCpuJob->cmd_list[currentCpuJob->current_cmd_int].second; //cmdVec[cmdVecidx].second;

        cout << "op andopram" << endl;
        cout << op << " " << opparam << endl;
        //execute

        currentCpuJob->current_cmd_int++;
        if (currentCpuJob->current_cmd_int == currentCpuJob->cmd_list.size())
        {
            currentCpuJob->cmdComplete = true;
        }
    }
}