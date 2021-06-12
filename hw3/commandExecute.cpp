#include "commandExecute.h"

void commandExecute(Job *&currentCpuJob, deque<Job *> *run_queue, list<Job *> &sleepList, list<Job *> &ioWaitList, list<Job *> &runningJobs_list, int currentCycle, PhysicalMemoryTree &physicalMemory, string page, int &page_fault) //, list<PageBundle *> *all_pages, PageBundle **physicalMem, string sched_opt, string page_opt, int &aid, int physicalNum, int currentCycle, int &page_fault);
{
    //cout << "start" << endl;
    if (currentCpuJob->pid != -1)
    {

        //vector<pair<int, int> > cmdVec = currentCpuJob->cmd_list;
        //int cmdVecidx = currentCpuJob->current_cmd_int;
        //cout << "assign" << endl;

        int op = currentCpuJob->cmd_list[currentCpuJob->current_cmd_int].first;       //cmdVec[cmdVecidx].first;
        int opparam = currentCpuJob->cmd_list[currentCpuJob->current_cmd_int].second; //cmdVec[cmdVecidx].second;

        //cout << "op and opram" << endl;
        //cout << op << " " << opparam << endl;
        //execute

        if (op == 0)
        {
            //memory allocation
            //only allocate on virtual memory
            cmd_malloc(currentCpuJob, opparam);
        }
        else if (op == 1)
        {
            //memory access
            cmd_memAccess(currentCpuJob, physicalMemory, opparam, currentCycle, page, page_fault, run_queue, sleepList, ioWaitList);
        }
        else if (op == 2)
        {
            //memory release
            cmd_memFree(currentCpuJob, physicalMemory, opparam);
        }
        else if (op == 3)
        {
            //non-memory instruction
            //pass
        }
        else if (op == 4)
        {
            //sleep
            if (currentCpuJob->current_cmd_int == currentCpuJob->cmd_list.size() - 1) //final op case
            {
                //dummy
            }
            else
            {
                opSleep(currentCpuJob, opparam, sleepList);
            }
            //after go to sleep queue,  show log that op is running and sent currentJob to sleequeue
        }
        else if (op == 5)
        {
            if (currentCpuJob->current_cmd_int == currentCpuJob->cmd_list.size() - 1) //final opcase
            {
                //dummy
            }
            else
            {
                opIOwait(currentCpuJob, ioWaitList);
            }
            //after go block, show log that op is running and sent currentJob to sleepqueue
            //iowait
        }

        currentCpuJob->current_cmd_int++;
        if (currentCpuJob->current_cmd_int == currentCpuJob->cmd_list.size())
        {
            currentCpuJob->cmdComplete = true;
        }
    }
}