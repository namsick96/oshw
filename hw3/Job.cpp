#include "Job.h"
//#include <iostream>

void Job::setData(int vmemSize, int pageSize)
{

    pageTablePagesize = pageSize;
    pageTablePageNum = vmemSize / pageSize;
    pageTableFinalPageID = 0;
    pageTable = new VirtualMemoryPageTable(pageTablePageNum, pageTablePagesize);

    for (int k = 0; k < pageTablePageNum; k++)
    {
        pageTable->pageIDv.push_back(-1);
        pageTable->allocationIDv.push_back(-1);
        pageTable->validBitv.push_back(-1);
        pageTable->refereceBitv.push_back(0);
        pageTable->refereceBytev.push_back(0);
        //pageTable->table[k] = nullptr;
        //pageTable->table[k].isDummy = true;
    }
    //printf("setDone2\n");

    ifstream process;
    string process_name = directory + "/" + file_name;
    process.open(process_name.c_str());

    //printf("setDone3\n");

    process >> cmd_num;
    for (int i = 0; i < cmd_num; i++)
    {
        int op;
        int val;
        process >> op >> val;

        pair<int, int> oneline = make_pair(op, val);
        cmd_list.push_back(oneline);
        //printf("setDone3\n");
    }
    process.close();
    current_cmd_int = 0;
    //printf("Set end\n");
}