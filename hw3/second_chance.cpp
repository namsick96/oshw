#include "second_chance.h"

bool cmpAllocID(const pair<Job *, int> &a, const pair<Job *, int> &b)
{
    return a.second < b.second;
}

void second_chance(Job *&currentCpuJob, PhysicalMemoryTree &physicalMemory, deque<Job *> *run_queue, list<Job *> &sleepList, list<Job *> &ioWaitList)
{

    //allocid 작은 순서대로 정렬하기
    list<pair<Job *, int> >::iterator job_iter;
    vector<pair<Job *, int> > sorted_allocid;
    for (job_iter = physicalMemory.allocated_list.begin(); job_iter != physicalMemory.allocated_list.end(); job_iter++)
    {
        pair<Job *, int> temp = *job_iter;
        sorted_allocid.push_back(temp);
    }
    sort(sorted_allocid.begin(), sorted_allocid.end(), cmpAllocID);

    bool envictOccur = false;
    Job *envictJob;
    int envictNodeAllocID;
    int envictNodePageID;

    //first round;
    for (int k = 0; k < sorted_allocid.size(); k++)
    {
        Job *tempJob = sorted_allocid[k].first;
        int tempAlloc = sorted_allocid[k].second;
        for (int i = 0; i < tempJob->pageTablePageNum; i++)
        {
            if (tempJob->pageTable->allocationIDv[i] == tempAlloc && tempJob->pageTable->refereceBitv[i] == 0)
            {
                //physical memory allocation disable!
                envictOccur = true;
                //tempJob->pageTable->allocationIDv[i] = -1;
                tempJob->pageTable->validBitv[i] = 0;
                envictNodePageID = tempJob->pageTable->pageIDv[i];
                //envict대상
            }
            else if (tempJob->pageTable->allocationIDv[i] == tempAlloc && tempJob->pageTable->refereceBitv[i] == 1)
            {
                tempJob->pageTable->refereceBitv[i] = 0;
            }
        }
        if (envictOccur)
        {
            envictJob = tempJob;
            envictNodeAllocID = tempAlloc;
            break;
        }
    }
    //second round
    if (!envictOccur)
    {
        for (int k = 0; k < sorted_allocid.size(); k++)
        {
            Job *tempJob = sorted_allocid[k].first;
            int tempAlloc = sorted_allocid[k].second;
            for (int i = 0; i < tempJob->pageTablePageNum; i++)
            {
                if (tempJob->pageTable->allocationIDv[i] == tempAlloc && tempJob->pageTable->refereceBitv[i] == 0)
                {
                    //physical memory allocation disable!
                    envictOccur = true;
                    //tempJob->pageTable->allocationIDv[i] = -1;
                    tempJob->pageTable->validBitv[i] = 0;
                    envictNodePageID = tempJob->pageTable->pageIDv[i];
                    //envict대상
                }
                else if (tempJob->pageTable->allocationIDv[i] == tempAlloc && tempJob->pageTable->refereceBitv[i] == 1)
                {
                    tempJob->pageTable->refereceBitv[i] = 0;
                }
            }
            if (envictOccur)
            {
                envictJob = tempJob;
                envictNodeAllocID = tempAlloc;
                break;
            }
        }
    }

    //vector에서 정해진 pair physicalMemory.allocated_list에서도 remove해주기
    physicalMemory.allocated_list.remove(make_pair(envictJob, envictNodeAllocID));

    //physical memory node도 envict해주기
    TreeNode *targetPhysicalNode = physicalMemory.findAllocID(physicalMemory.root, envictNodeAllocID);

    targetPhysicalNode->allocationID = -1;
    targetPhysicalNode->pageID = -1;
    physicalMemory.wrapUp(targetPhysicalNode); //버디 시스템으로 메모리 wrapup

    //while문과 나머지 값으로 인덱싱 해서 원형 순회 구현. allocid작은 순서대로 순회. 만약 해당 Job의 allocid가 배정된 page의 reference bit가 0이면 해당 페이지 envict하고 바로 리턴, 아니면 reference bit 0으로 두기.
}