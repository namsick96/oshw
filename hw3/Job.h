//jihoon JOB def

#include <string>
#include <fstream>
#include <vector>
#include <list>
#include <utility>
#include <math.h>

//#include "VirtualMemoryPageTable.h"

#ifndef __JOB_H__
#define __JOB_H__

using namespace std;
/*
class PageTableEntry
{
    friend class VirtualMemoryPageTable;
    friend class Job;

public:
    bool isDummy;
    int pageID;
    bool validBit;
    int allocationID;
    unsigned char referenceBit;
    unsigned char reference_byte;
    PageTableEntry(int page_id)
    {
        pageID = page_id;
        referenceBit = 0;
        reference_byte = 0;
        validBit = false;
    }
};
*/
class VirtualMemoryPageTable
{
    friend class Job;
    // friend class PageTableEntry;

public:
    struct PageTableEntry
    {
        bool isDummy;
        int pageID;
        bool validBit;
        int allocationID;
        unsigned char referenceBit = 0;
        unsigned char reference_byte = 0;
    };
    int pageNum;
    int pageSize;
    vector<int> pageIDv;
    vector<int> validBitv;
    vector<int> allocationIDv;
    vector<unsigned char> refereceBitv;
    vector<unsigned char> refereceBytev;
    //PageTableEntry *table;
    VirtualMemoryPageTable(int pagenum, int pagesize)
    {
        pageNum = pagenum;
        pageSize = pagesize;
    }
};

class Job
{
    friend class VirtualMemoryPageTable;
    //friend class PageTableEntry;

public:
    int startCycle; // when process come in to run queue
    int cmd_num;    // number of command in process
    int pid;        // process id
    int priority;   // priority

    bool cpuEmpty;    // flag of current cpu should be empty after print
    bool cmdComplete; // flag of command completed

    int current_cmd_int; // check which command's turn(int)
    //vector<int *>::iterator current_cmd; // check which command's turn(iterator)
    vector<pair<int, int> > cmd_list; // list of command

    //int memoryCursor;  // cursor of pageTable
    //int pageTableSize; // size of pageTable
    //PageBundle **pageTable_aid;   // page table (aid)
    //PageBundle **pageTable_valid; // page table (valid)
    //int recentAid;     // for print memory log
    //int recentPageNum; // for print memory log

    int pageTableidx;
    int pageTablePageNum;
    int pageTablePagesize;
    int pageTableFinalPageID;
    int recentUsedPageNum; //pageCount at cmd_memFree
    VirtualMemoryPageTable *pageTable;

    //int recentAid; // for print memory log
    //int recentPageNum; // for print memory log

    string directory; // save directory
    string file_name; // process file name

    int turnAroundTime; //whole process start to end time with waiting time
    int waitingTime;    // for waiting
    int responseTime;   // for eval
    int realStartTime;  //real start cycle
    int runningTime;    //running tick. initiate when scheduled.
    int sleepTime;      // sleep time counter
    int IOwaitTime;     //IOwait time counter
    int timeQuantum;    // for count running time in cpu

    bool sleepState;
    bool IOwaitState;

    Job(int startInput, string dir, string filename, int priority_num, int pid_count)
    {
        startCycle = startInput;
        file_name = filename;
        priority = priority_num;
        directory = dir;
        pid = pid_count;
        cpuEmpty = false;
        cmdComplete = false;
        timeQuantum = 10;

        pageTableidx = 0;

        //recentAid = -1;
        //recentPageNum = -1;

        turnAroundTime = 0;
        waitingTime = 0;
        responseTime = -1;
        realStartTime = -1;
        runningTime = -1;
        sleepTime = 0;
        IOwaitTime = 0;
        cmdComplete = false;

        sleepState = false;
        IOwaitState = false;
    }

    Job()
    {
    }

    void setData(int vmemSize, int pageSize);
};

#endif //__JOB_H__