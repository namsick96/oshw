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
    currentCpuJob->recentUsedPageNum = pageCount;
    //physical memory먼저 unload시키기
    int targetAllocID = currentCpuJob->pageTable->allocationIDv[currentCpuJob->pageTableidx];
    TreeNode *targetPhysicalNode = physicalMemory.findAllocID(physicalMemory.root, targetAllocID);
    physicalMemory.allocID_stack.remove(targetPhysicalNode->allocationID);
    physicalMemory.allocated_list.remove(make_pair(currentCpuJob, targetPhysicalNode->allocationID));
    //wrapup method
    targetPhysicalNode->allocationID = -1;
    targetPhysicalNode->pageID = -1;
    physicalMemory.wrapUp(targetPhysicalNode); //버디 시스템으로 메모리 wrapup

    //virtual memory unload시키기
    for (int k = currentCpuJob->pageTableidx; k < currentCpuJob->pageTableidx + pageCount; k++)
    {
        currentCpuJob->pageTable->pageIDv[k] = -1;
        currentCpuJob->pageTable->allocationIDv[k] = -1;
        currentCpuJob->pageTable->refereceBitv[k] = 0;
        currentCpuJob->pageTable->refereceBytev[k] = 0;
        currentCpuJob->pageTable->validBitv[k] = -1;
    }
}