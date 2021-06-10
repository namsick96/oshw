#include "lru.h"

//다시하기
//virtual memroy안에서만 페이지 빼고 넣는거임. 피지컬 메모리에서 해야함.

void lru(Job *&currentCpuJob, PhysicalMemoryTree &physicalMemory)
{
    //printf("lru case0\n");
    //envict target
    int envictallocID = physicalMemory.allocID_stack.front();
    physicalMemory.allocID_stack.pop_front();

    //printf("lru case1\n");

    list<pair<Job *, int> >::iterator allocList_iter;
    for (allocList_iter = physicalMemory.allocated_list.begin(); allocList_iter != physicalMemory.allocated_list.end(); allocList_iter++)
    {
        pair<Job *, int> target = *allocList_iter;
        if (target.second == envictallocID)
        {
            break;
        }
    }
    Job *envictJob = allocList_iter->first;
    //printf("lru case2\n");

    //virtual memory envict

    for (int k = 0; k < envictJob->pageTablePageNum; k++)
    {
        if (envictJob->pageTable->allocationIDv[k] == envictallocID)
        {
            //envictJob->pageTable->allocationIDv[k] = -1;
            envictJob->pageTable->validBitv[k] = 0;
        }
    }
    //printf("lru case3\n");
    //physical memory envict
    TreeNode *envictNode = physicalMemory.findAllocID(physicalMemory.root, envictallocID);

    //initialized stack and list
    if (envictNode != nullptr)
    {
        physicalMemory.allocID_stack.remove(envictNode->allocationID);
        physicalMemory.allocated_list.remove(make_pair(envictJob, envictNode->allocationID));

        //printf("lru case4\n");
        //initialize
        envictNode->allocationID = -1;
        envictNode->pageID = -1;
        physicalMemory.wrapUp(envictNode); //wrap up physical node with buddy system
    }
}