#include "opSleep.h"

void opSleep(Job *&currentCpuJob, int opParam, list<Job *> &sleepList)
{
    currentCpuJob->sleepTime = opParam;
    sleepList.push_back(currentCpuJob);
    currentCpuJob->sleepState = true;
    /*
    if (currentCpuJob->current_cmd_int < currentCpuJob->cmd_list.size())
    {
        sleepList.push_back(currentCpuJob);
    }*/
}