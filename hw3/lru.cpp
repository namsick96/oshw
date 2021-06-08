#include "lru.h"

int lru(Job *&currentCpuJob, PhysicalMemoryTree &physicalMemory)
{
    //envict target
    int envictPageID = physicalMemory.pageID_stack.front();
    physicalMemory.pageID_stack.pop_front();

    //virtual page setup
    bool startIdxFind = false;
    int pageCount = 0;
    for (int i = 0; i < currentCpuJob->pageTablePageNum; i++)
    {
        if (currentCpuJob->pageTable->pageIDv[i] == envictPageID)
        {
            if (!startIdxFind)
            {
                currentCpuJob->pageTableidx = i;
                startIdxFind = true;
            }
            pageCount++;
        }
    }
    //physical memory envict
    TreeNode *envictNode = physicalMemory.findPage(physicalMemory.root, envictPageID);
    envictNode->allocationID = -1;
    envictNode->pageID = -1;
    physicalMemory.wrapUp(envictNode);

    //virtual memory envict
    for (int k = currentCpuJob->pageTableidx; k < currentCpuJob->pageTableidx + pageCount; k++)
    {
        currentCpuJob->pageTable->allocationIDv[k] = -1;
        currentCpuJob->pageTable->validBitv[k] = 0;
    }
}