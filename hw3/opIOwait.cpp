#include "opIOwait.h"

void opIOwait(Job *&currentCpuJob, list<Job *> &ioWaitList)
{
    ioWaitList.push_back(currentCpuJob);
    currentCpuJob->sleepState = true;
}