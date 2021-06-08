#include "cmd_malloc.h"
#include <iostream>

void cmd_malloc(Job *&currentCpuJob, int opparam)
{
    currentCpuJob->pageTableidx = -1;

    for (int i = 0; i < currentCpuJob->pageTablePageNum; i++)
    {
        //currentCpuJob->pageTable->table[i].isDummy = 1;
        if (currentCpuJob->pageTable->pageIDv[i] == -1)
        {
            bool allNull = true;
            for (int k = 1; k < opparam; k++)
            {
                if (currentCpuJob->pageTable->pageIDv[i + k] != -1)
                {
                    allNull = false;
                    break;
                }
            }
            if (allNull)
            { //allocate in virtual memory not in physical memory
                for (int k = 0; k < opparam; k++)
                {
                    currentCpuJob->pageTable->pageIDv[i + k] = currentCpuJob->pageTableFinalPageID++;
                    currentCpuJob->pageTableFinalPageID++;
                    cout << "virtual Allocated" << endl;
                }
                break; // allocation Done
            }
        }
    }
}