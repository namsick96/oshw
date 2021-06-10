#include "cmd_memAccess.h"

void cmd_memAccess(Job *&currentCpuJob, PhysicalMemoryTree &physicalMemory, int opparam, int currentCycle, string page, int &page_fault, deque<Job *> *run_queue, list<Job *> &sleepList, list<Job *> &ioWaitList)
{
    // virtualmemory find page

    //printf("memAccess start\n");

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
    if (currentCpuJob->pageTable->validBitv[currentCpuJob->pageTableidx] == 1)
    {
        //physical memory have page find it!
        int targetAllocID = currentCpuJob->pageTable->allocationIDv[currentCpuJob->pageTableidx];
        TreeNode *targetPhysicalNode = physicalMemory.findAllocID(physicalMemory.root, targetAllocID);
        for (int k = currentCpuJob->pageTableidx; k < currentCpuJob->pageTableidx + pageCount; k++)
        {
            currentCpuJob->pageTable->refereceBitv[k] = 1;
        }
        physicalMemory.allocID_stack.remove(targetAllocID);
        physicalMemory.allocID_stack.push_back(targetAllocID);
    }
    else
    {
        //there is no pages on physocal memory
        page_fault++; //increase pagefault

        //test
        //printf("page fault: %d\n", page_fault);

        //pageID == opparam in virtualMemory
        //start at currentCpuJob->pageTableidx and count is pageCount
        //cout << "hihi" << endl;
        //test
        //printf("root allocid: %d, frameNum: %d, pageCount: %d", physicalMemory.root->allocationID, physicalMemory.root->frameNum, pageCount);

        TreeNode *targetPhysicalMemBlock = physicalMemory.findProperNode(physicalMemory.root, pageCount);
        /*
        cout << targetPhysicalMemBlock->allocationID << endl
             << targetPhysicalMemBlock->frameNum << endl
             << pageCount << endl;

        printf("temp\n");
        printf("%d and %d and %d", targetPhysicalMemBlock->allocationID, targetPhysicalMemBlock->frameNum, pageCount);*/

        while (targetPhysicalMemBlock == nullptr)
        {
            //lru먼저 만들기
            //lru(currentCpuJob, physicalMemory);
            //targetPhysicalMemBlock = physicalMemory.findProperNode(physicalMemory.root, pageCount);

            //page교체 알고리즘 쓰기 빈곳이 없어요
            printf("replacement algorithm start\n");
            if (strcmp(page.c_str(), "sampled") == 0)
            {
                //physicalmemory에서 aid 올라가 있는 것들 다 확인한다음 aid값 같은 페이지 방문해서 reference byte확인해야함.
                //그리고 확인 되면 가장 작은거 envict;
                sampled(currentCpuJob, physicalMemory, run_queue, sleepList, ioWaitList);
                targetPhysicalMemBlock = physicalMemory.findProperNode(physicalMemory.root, pageCount);
                //sampled lru page교체 알고리즘
            }
            else if (strcmp(page.c_str(), "clock") == 0)
            {

                second_chance(currentCpuJob, physicalMemory, run_queue, sleepList, ioWaitList);
                targetPhysicalMemBlock = physicalMemory.findProperNode(physicalMemory.root, pageCount);
            }
            else
            {
                //lru page 교체 알고리즘
                lru(currentCpuJob, physicalMemory);
                targetPhysicalMemBlock = physicalMemory.findProperNode(physicalMemory.root, pageCount);
            }
        }
        if (targetPhysicalMemBlock != nullptr)
        {
            //physical memory update
            targetPhysicalMemBlock->allocationID = physicalMemory.physical_final_allocationID;
            targetPhysicalMemBlock->pageID = opparam;
            physicalMemory.allocID_stack.push_back(targetPhysicalMemBlock->allocationID);
            physicalMemory.allocated_list.push_back(make_pair(currentCpuJob, physicalMemory.physical_final_allocationID));
            //virtual memory update
            for (int k = currentCpuJob->pageTableidx; k < currentCpuJob->pageTableidx + pageCount; k++)
            {
                currentCpuJob->pageTable->pageIDv[k] = opparam;
                currentCpuJob->pageTable->allocationIDv[k] = physicalMemory.physical_final_allocationID;
                currentCpuJob->pageTable->refereceBitv[k] = 1;
                currentCpuJob->pageTable->validBitv[k] = 1;
            }
            physicalMemory.physical_final_allocationID++;
        }
    }
}