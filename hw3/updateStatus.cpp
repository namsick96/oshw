#include <list>
#include <deque>
#include <vector>
#include "Job.h"
#include "updateStatus.h"

void updateStatus(Job *&currentCpuJob, deque<Job *> *run_queue, list<Job *> &runningJobs_list, list<Job *> &sleepList, list<Job *> &ioWaitList, list<Job *> &endProcess, int currentCycle, int &timeInterval, Job *&nullJob, PhysicalMemoryTree &physicalMemory, string page)
{

    timeInterval++; //for lru-sampled
    // timeInterval==8이 되는 순간 runque, sleeplist, iowaitlist에 있는 프로세스들 reference bit가 referencbyte의 msb가 됩니다.
    //여기서 구현하기
    if (strcmp(page.c_str(), "sampled") == 0)
    {
        if (timeInterval % 8 == 0)
        {
            //currentCPuJob
            for (int k = 0; k < currentCpuJob->pageTablePageNum; k++)
            {
                if (currentCpuJob->pageTable->pageIDv[k] == -1)
                {
                    unsigned char temp_ref_bit = currentCpuJob->pageTable->refereceBitv[k];
                    unsigned char temp_ref_byte = currentCpuJob->pageTable->refereceBytev[k];
                    temp_ref_byte = temp_ref_byte >> 1;
                    temp_ref_byte = temp_ref_byte | (temp_ref_bit << 7);
                    currentCpuJob->pageTable->refereceBytev[k] = temp_ref_byte;
                    currentCpuJob->pageTable->refereceBitv[k] = 0;
                }
            }
            //run_queue
            for (int k = 0; k < 10; k++)
            {
                if (!run_queue[k].empty())
                {
                    for (int i = 0; i < run_queue[k].size(); i++)
                    {
                        Job *job = run_queue[k][i];
                        for (int q = 0; q < job->pageTablePageNum; q++)
                        {
                            if (job->pageTable->pageIDv[q] == -1)
                            {
                                unsigned char temp_ref_bit = job->pageTable->refereceBitv[q];
                                unsigned char temp_ref_byte = job->pageTable->refereceBytev[q];
                                temp_ref_byte = temp_ref_byte >> 1;
                                temp_ref_byte = temp_ref_byte | (temp_ref_bit << 7);
                                job->pageTable->refereceBytev[q] = temp_ref_byte;
                                job->pageTable->refereceBitv[q] = 0;
                            }
                        }
                    }
                }
            }
            //sleepList
            list<Job *>::iterator job_iter;
            for (job_iter = sleepList.begin(); job_iter != sleepList.end(); job_iter++)
            {
                Job *job = *job_iter;
                for (int q = 0; q < job->pageTablePageNum; q++)
                {
                    if (job->pageTable->pageIDv[q] == -1)
                    {
                        unsigned char temp_ref_bit = job->pageTable->refereceBitv[q];
                        unsigned char temp_ref_byte = job->pageTable->refereceBytev[q];
                        temp_ref_byte = temp_ref_byte >> 1;
                        temp_ref_byte = temp_ref_byte | (temp_ref_bit << 7);
                        job->pageTable->refereceBytev[q] = temp_ref_byte;
                        job->pageTable->refereceBitv[q] = 0;
                    }
                }
            }

            //ioWaitlist
            for (job_iter = ioWaitList.begin(); job_iter != ioWaitList.end(); job_iter++)
            {
                Job *job = *job_iter;
                for (int q = 0; q < job->pageTablePageNum; q++)
                {
                    if (job->pageTable->pageIDv[q] == -1)
                    {
                        unsigned char temp_ref_bit = job->pageTable->refereceBitv[q];
                        unsigned char temp_ref_byte = job->pageTable->refereceBytev[q];
                        temp_ref_byte = temp_ref_byte >> 1;
                        temp_ref_byte = temp_ref_byte | (temp_ref_bit << 7);
                        job->pageTable->refereceBytev[q] = temp_ref_byte;
                        job->pageTable->refereceBitv[q] = 0;
                    }
                }
            }

            //currentCpuJob, run_queue sleepList, ioWaitlist에 있는 Job들 referece bit를 referece byte의 msb로 옮겨주기.

            //page id가 -1이 아닌 page table의 인덱스를 확인한 후 해당 reference bit를 reference byte의 msb로 만들어 줍니다.
        }
    }

    if (currentCpuJob->pid != -1)
    {
        // for all Job add runningTime
        currentCpuJob->runningTime++;

        // check response time by real start time -starting time
        if (currentCpuJob->responseTime == -1)
        {
            currentCpuJob->responseTime = currentCycle - currentCpuJob->startCycle;
            currentCpuJob->realStartTime = currentCycle;
        }

        // count waiting time
        for (int k = 0; k < 10; k++)
        {
            if (!run_queue[k].empty())
            {
                for (int i = 0; i < run_queue[k].size(); i++)
                {
                    run_queue[k][i]->waitingTime++;
                    run_queue[k][i]->timeQuantum--;
                }
            }
        }
        //if currentCpuJob is done.
        if (currentCpuJob->cmdComplete)
        {
            //메모리 해제
            //currentCpuJob의 모든 page id 저장.  돌아가면서 해제하기.

            for (int k = 0; k < currentCpuJob->pageTablePageNum; k++)
            {
                int freePageID = currentCpuJob->pageTable->pageIDv[k];

                if (freePageID != -1 && currentCpuJob->pageTable->validBitv[k] == 1)
                {
                    cmd_memFree(currentCpuJob, physicalMemory, currentCpuJob->pageTable->pageIDv[k]);
                }
            }

            endProcess.push_back(currentCpuJob);
            currentCpuJob = nullJob;

            //메모리 해제 구현하기
        }

        //if currentCpujob goes to sleeping
        if (currentCpuJob->sleepState)
        {
            currentCpuJob->sleepState = false;
            currentCpuJob = nullJob;
        }

        //if currentCpuJob goes to ioblock
        if (currentCpuJob->IOwaitState)
        {
            currentCpuJob->IOwaitState = false;
            currentCpuJob = nullJob;
        }
    }

    // if (currentCpuJob->cmdComplete)
    // {
    //     endProcess.push_back(currentCpuJob);
    //     currentCpuJob = nullJob;
    // }
    // else
    // {
    //     timeInterval++;
    // }
}