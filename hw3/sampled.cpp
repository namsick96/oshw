#include "sampled.h"

bool cmpAllocID2(const pair<Job *, int> &a, const pair<Job *, int> &b)
{
    return a.second < b.second;
}

void sampled(Job *&currentCpuJob, PhysicalMemoryTree &physicalMemory, deque<Job *> *run_queue, list<Job *> &sleepList, list<Job *> &ioWaitList)
{

    //allocid 작은 순서대로 정렬하기
    list<pair<Job *, int> >::iterator job_iter;
    vector<pair<Job *, int> > sorted_allocid;
    for (job_iter = physicalMemory.allocated_list.begin(); job_iter != physicalMemory.allocated_list.end(); job_iter++)
    {
        pair<Job *, int> temp = *job_iter;
        sorted_allocid.push_back(temp);
    }
    sort(sorted_allocid.begin(), sorted_allocid.end(), cmpAllocID2);

    Job *envictJob;
    int envictNodeAllocID;
    int envictNodePageID;

    vector<unsigned char> referencebyte_vector;

    for (int k = 0; k < sorted_allocid.size(); k++)
    {
        Job *targetJob = sorted_allocid[k].first;
        int targetAllocID = sorted_allocid[k].second;
        for (int i = 0; i < targetJob->pageTablePageNum; i++)
        {
            if (targetJob->pageTable->allocationIDv[k] == targetAllocID)
            {
                referencebyte_vector.push_back(targetJob->pageTable->refereceBytev[i]);
                break;
            }
        }
    }

    //compare in refernecbyte vector which on is smallest;
    int envict_index;
    unsigned char tempUC = 255;
    for (int k = 0; k < referencebyte_vector.size(); k++)
    {
        if (referencebyte_vector[k] < tempUC)
        {
            envict_index = k;
            tempUC = referencebyte_vector[k];
        }
    }

    //envict Job is in sorted_allocid envict_index th node
    envictJob = sorted_allocid[envict_index].first;
    envictNodeAllocID = sorted_allocid[envict_index].second;

    //virtual memory unallocate
    for (int k = 0; k < envictJob->pageTablePageNum; k++)
    {
        if (envictJob->pageTable->allocationIDv[k] == envictNodeAllocID)
        {
            envictJob->pageTable->allocationIDv[k] = -1;
            envictJob->pageTable->validBitv[k] = 0;
            envictJob->pageTable->refereceBitv[k] = 0;
        }
    }

    //physical memory unallocate
    physicalMemory.allocated_list.remove(make_pair(envictJob, envictNodeAllocID));

    //physical memory node도 envict해주기
    TreeNode *targetPhysicalNode = physicalMemory.findAllocID(physicalMemory.root, envictNodeAllocID);

    targetPhysicalNode->allocationID = -1;
    targetPhysicalNode->pageID = -1;
    physicalMemory.wrapUp(targetPhysicalNode); //버디 시스템으로 메모리 wrapup
}