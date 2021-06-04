//jihoon JOB def

#include <string>
#include <fstream>
#include <vector>

//#include "PageBundle.h"

#ifndef __JOB_H__
#define __JOB_H__

using namespace std;

class Job
{

public:
    int startCycle;  // when process come in to run queue
    int cmd_num;     // number of command in process
    int pid;         // process id
    int priority;    // priority
    int sleepTime;   // sleep time counter
    int timeQuantum; // for count running time in cpu

    bool cpuEmpty;    // flag of current cpu should be empty after print
    bool cmdComplete; // flag of command completed

    int current_cmd_int;                 // check which command's turn(int)
    vector<int *>::iterator current_cmd; // check which command's turn(iterator)
    vector<int *> cmd_list;              // list of command

    int memoryCursor;  // cursor of pageTable
    int pageTableSize; // size of pageTable
    //PageBundle **pageTable_aid;   // page table (aid)
    //PageBundle **pageTable_valid; // page table (valid)
    int recentAid;     // for print memory log
    int recentPageNum; // for print memory log

    string directory; // save directory
    string file_name; // process file name

    int waitingTime;  // for evaluate
    int responseTime; // for evaluate

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

        memoryCursor = 0;

        recentAid = -1;
        recentPageNum = -1;

        waitingTime = 0;
        responseTime = -1;
    }

    Job()
    {
    }

    void setData(int vmemSize, int pageSize);

    void printCmd();
};

#endif //__JOB_H__