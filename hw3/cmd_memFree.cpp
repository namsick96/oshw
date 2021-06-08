#include "cmd_memFree.h"

void cmd_memFree(Job *&currentCpuJob, PhysicalMemoryTree &physicalMemory, int opparam)
{
    // virtualmemory find page

    bool startIdxFind = false;
    int pageCount = 0;
    for (int i = 0; i < currentCpuJob->pageTablePageNum; i++)
    {
        if (currentCpuJob->pageTable->pageIDv[i] == opparam)
        {
            if (!startIdxFind)
            {
                currentCpuJob->pageTableidx = i;
                startIdxFind = true;
            }
            pageCount++;
        }
    }
    //physical memory먼저 unload시키기
    int targetPageID = currentCpuJob->pageTable->pageIDv[currentCpuJob->pageTableidx];
    TreeNode *targetPhysicalNode = physicalMemory.findPage(physicalMemory.root, targetPageID);
    //buddy is not working
    //wrapup method
    targetPhysicalNode->allocationID = -1;
    targetPhysicalNode->pageID = -1;
    physicalMemory.wrapUp(targetPhysicalNode);

    //virtual memory unload시키기
    for (int k = currentCpuJob->pageTableidx; k < currentCpuJob->pageTableidx + pageCount; k++)
    {
        currentCpuJob->pageTable->pageIDv[k] = -1;
        currentCpuJob->pageTable->allocationIDv[k] = -1;
        currentCpuJob->pageTable->refereceBitv[k] = 0;
        currentCpuJob->pageTable->refereceBytev[k] = 0;
        currentCpuJob->pageTable->validBitv[k] = 0;
    }
}