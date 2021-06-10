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

    //printf("cmd_memFree: case1\n");

    currentCpuJob->recentUsedPageNum = pageCount;
    //physical memory먼저 unload시키기
    int targetAllocID = currentCpuJob->pageTable->allocationIDv[currentCpuJob->pageTableidx];
    if (targetAllocID != -1)
    {
        //printf("cmd_memFree: casw11: %d\n", targetAllocID);
        TreeNode *targetPhysicalNode = physicalMemory.findAllocID(physicalMemory.root, targetAllocID);
        if (targetPhysicalNode == nullptr)
        {
            //printf("cmd_memFree: case12\n");
        }
        //printf("cmd_memFree: case2\n");
        //physical memory FREE
        bool allocIDtrue = false;
        list<int>::iterator temp_iter;
        for (temp_iter = physicalMemory.allocID_stack.begin(); temp_iter != physicalMemory.allocID_stack.end(); temp_iter++)
        {
            int temp = *temp_iter;
            //printf("cmd_memFree: case201\n");
            if (temp == targetPhysicalNode->allocationID)
            {
                allocIDtrue = true;
            }
            //printf("cmd_memFree: case201\n");
        }
        //printf("cmd_memFree: case21\n");
        if (allocIDtrue)
        {
            physicalMemory.allocID_stack.remove(targetPhysicalNode->allocationID);
        }
        //physicalMemory.allocID_stack.remove(targetPhysicalNode->allocationID);

        bool allocatedListtrue = false;
        list<pair<Job *, int> >::iterator temp2_iter;
        for (temp2_iter = physicalMemory.allocated_list.begin(); temp2_iter != physicalMemory.allocated_list.end(); temp2_iter++)
        {
            pair<Job *, int> temp = *temp2_iter;
            Job *tempJob = temp.first;
            int tempInt = temp.second;
            if (tempJob == currentCpuJob && tempInt == targetPhysicalNode->allocationID)
            {
                allocatedListtrue = true;
            }
        }
        //printf("cmd_memFree: case22\n");
        if (allocatedListtrue)
        {
            physicalMemory.allocated_list.remove(make_pair(currentCpuJob, targetPhysicalNode->allocationID));
        }
        //physicalMemory.allocated_list.remove(make_pair(currentCpuJob, targetPhysicalNode->allocationID));

        //wrapup method
        //printf("cmd_memFree: case3\n");
        targetPhysicalNode->allocationID = -1;
        targetPhysicalNode->pageID = -1;
        physicalMemory.wrapUp(targetPhysicalNode); //버디 시스템으로 메모리 wrapup

        //printf("cmd_memFree: case4\n");
    }
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