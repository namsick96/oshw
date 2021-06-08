#include "cmd_memAccess.h"

void cmd_memAccess(Job *&currentCpuJob, PhysicalMemoryTree &physicalMemory, int opparam, int currentCycle, string page, int &page_fault)
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

    //physical memory find page(frame)
    if (currentCpuJob->pageTable->validBitv[currentCpuJob->pageTableidx])
    {
        //physical memory have page find it!
        int targetPageID = currentCpuJob->pageTable->pageIDv[currentCpuJob->pageTableidx];
        TreeNode *targetPhysicalNode = physicalMemory.findPage(physicalMemory.root, targetPageID);
        for (int k = currentCpuJob->pageTableidx; k < currentCpuJob->pageTableidx + pageCount; k++)
        {
            currentCpuJob->pageTable->refereceBitv[k] = 1;
        }
        physicalMemory.pageID_stack.remove(targetPageID);
        physicalMemory.pageID_stack.push_back(targetPageID);
    }
    else
    {
        //there is no pages on physocal memory
        page_fault++; //increase pagefault

        //pageID == opparam in virtualMemory
        //start at currentCpuJob->pageTableidx and count is pageCount
        TreeNode *targetPhysicalMemBlock = physicalMemory.findProperNode(physicalMemory.root, pageCount);

        while (targetPhysicalMemBlock == nullptr)
        {
            //lru먼저 만들기
            lru(currentCpuJob, physicalMemory);
            targetPhysicalMemBlock = physicalMemory.findProperNode(physicalMemory.root, pageCount);

            /*
            //page교체 알고리즘 쓰기 빈곳이 없어요
            if (strcmp(page.c_str(), "lru") == 0)
            {
                //lru page 교체 알고리즘
            }
            else if (strcmp(page.c_str(), "sampled") == 0)
            {
                //sampled lru page교체 알고리즘
            }
            else if (strcmp(page.c_str(), "clock") == 0)
            {
                //clock page교체 알고리즘
            }
            else
            {
                //lru page교체 알고리즘
            }
            */
        }
        if (targetPhysicalMemBlock != nullptr)
        {
            //physical memory update
            targetPhysicalMemBlock->allocationID = physicalMemory.physical_final_allocationID;
            targetPhysicalMemBlock->pageID = opparam;
            physicalMemory.pageID_stack.push_back(targetPhysicalMemBlock->pageID);
            //virtual memory update
            for (int k = currentCpuJob->pageTableidx; k < currentCpuJob->pageTableidx + pageCount; k++)
            {
                currentCpuJob->pageTable->allocationIDv[k] = physicalMemory.physical_final_allocationID;
                currentCpuJob->pageTable->refereceBitv[k] = 1;
                currentCpuJob->pageTable->validBitv[k] = 1;
            }
            physicalMemory.physical_final_allocationID++;
        }
    }
}