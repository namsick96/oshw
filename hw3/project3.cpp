//code by jihooan from may 29th to
//STL
#include <iostream>
#include <string.h>
#include <string>
#include <vector>
#include <list>
#include <stdio.h>
#include <fstream>
#include <unistd.h>
#include <queue>
//My
#include "Job.h"
#include "InputIO.h"
#include "scheduler.h"
#include "check_sleep.h"
#include "check_ioWait.h"
#include "check_jobToRun.h"

using namespace std;

int main(int argc, char *argv[])
{
    string page = "lru";
    char tempDir[256];
    getcwd(tempDir, 256);
    string dir = tempDir;

    int allocationID = 1;
    int timeInterval = 0; // time interval for lru-sample
    int page_fault = 0;
    int accessCounter = 0;

    //we save -page, -dir
    for (int i = 1; i < argc; i++)
    {
        string token = argv[i];
        int start = token.find("=") + 1;
        int option_length = token.size() - start;
        string option = token.substr(0, start - 1);
        string option_detail = token.substr(start, option_length);

        if (strcmp(option.c_str(), "-page") == 0)
        {
            page = option_detail;
            cout << page << "\n";
        }
        else if (strcmp(option.c_str(), "-dir") == 0)
        {
            dir = option_detail;
            cout << dir << "\n";
        }
    }

    //    save task
    int totalEventNum;
    int vmSize;
    int pmSize;
    int pageSize;
    string input_file = dir + "/" + "input";

    ifstream infile;
    infile.open(input_file.c_str());
    infile >> totalEventNum >> vmSize >> pmSize >> pageSize;
    cout << totalEventNum << vmSize << pmSize << pageSize;

    vector<Job *> jobs_list;
    list<Job *> runningJobs_list;
    vector<InputIO *> inputs_list;
    //vector<PageBundle*> pages_list;

    int pid_count = 0;
    for (int i = 0; i < totalEventNum; i++)
    {
        int startTime;
        string file_name;
        infile >> startTime >> file_name;

        if (strcmp(file_name.c_str(), "INPUT") == 0)
        {
            int target_pid;
            infile >> target_pid;
            inputs_list.push_back(new InputIO(startTime, target_pid));
        }
        else
        {
            int priority;
            infile >> priority;
            Job *newJob = new Job(startTime, dir, file_name, priority, pid_count);
            newJob->setData(vmSize, pageSize);
            jobs_list.push_back(newJob);
            pid_count++;
        }
    }

    queue<Job *> run_queue[10];
    list<Job *> sleepList;
    list<Job *> ioWaitList;
    list<Job *> endProcess;
    // list<int> lockList; 이건 뭘까
    /*
메모리 설정 넣자

*/
    // main loop
    int inProcessing = jobs_list.size();
    int currentCycle = -1;
    scheduler schedulerBot = scheduler(run_queue);
    Job *currentCpuJob = new Job(-1, "/", "NULL", -1, -1); //null job

    string schedule_output = dir + "/scheduler.txt";
    FILE *fp_sched = fopen(schedule_output.c_str(), "w");

    string memory_output = dir + "/memory.txt";
    FILE *fp_memory = fopen(memory_output.c_str(), "w");

    while (inProcessing > 0)
    {
        currentCycle++;

        // step 1
        check_sleep(run_queue, sleepList);

        // step 2
        check_ioWait(run_queue, ioWaitList, inputs_list, currentCycle);

        // step 3
        check_jobToRun(run_queue, runningJobs_list, jobs_list, currentCycle);

        // step 4
        currentCpuJob = schedulerBot.scheduling(currentCpuJob);
    }
}
/*
    int physicalMemorySize = pmemSize/pageSize;
    PageBundle** physicalMemory = new PageBundle*[physicalMemorySize];

    for(int i = 0 ; i < physicalMemorySize ; i++){
        if(i == 0){
            physicalMemory[i] = new PageBundle(physicalMemorySize, -1, 0);
            physicalMemory[i]->pbId = 0;
            physicalMemory[i]->parentId = -1;
        }else{
            physicalMemory[i] = nullptr;
        }
    }

    Task* nullTask = new Task(-1, ".", "null", -1);
    nullTask->burstTime = 10000000.0;
    nullTask->burstTime_exp = 10000000.0;
    nullTask->runningTime = -1;
    Task* currentCpuTask = nullTask;


    // main loop
    int inProcessing = all_task.size();
    int currentCycle = 0;
    scheduler schedulerBot = scheduler(sched, &run_queue, nullTask);


    string out_schedule = dir + "/scheduler.txt";
    FILE* fp_sched = fopen(out_schedule.c_str(), "w");

    string out_memory = dir + "/memory.txt";
    FILE* fp_memory = fopen(out_memory.c_str(), "w");

    while(inProcessing > 0){

        bool append = false;
        currentCycle++;

        // step 1
        bool append1 = check_sleep(&run_queue, &sleepList);

        // step 2
        bool append2 = check_ioWait(&run_queue, &ioWaitList, &all_input, currentCycle);

        // step 3
        bool append3 = check_taskToRun(&run_queue, &runningTask, &all_task, currentCycle);

        append = append1 || append2 || append3;

        // step 4
        currentCpuTask = schedulerBot.schedule(currentCpuTask, append);

        report_criteria(currentCpuTask, &run_queue, currentCycle);

        // step 5
        commandExecuter(currentCpuTask, &sleepList, &ioWaitList, &lockList, &runningTask, &all_pages, physicalMemory, sched, page, allocationID, physicalMemorySize, currentCycle, page_fault);

        if(currentCpuTask->pid != -1){
            int* runCmd = currentCpuTask->current_cmd.operator*();
            if(runCmd[0] == 1){
                accessCounter++;
            }
        }

        // step 6
        printLog(fp_sched, currentCpuTask, run_queue, sleepList, ioWaitList, currentCycle);
        printMemoryLog(fp_memory, currentCpuTask, &all_pages, physicalMemory, physicalMemorySize, &runningTask, currentCycle, page_fault);

        // step 7
        updateStatus(currentCpuTask, nullTask, &run_queue, &runningTask, &sleepList, &ioWaitList, &endProcess, physicalMemory, physicalMemorySize, currentCycle, timeInterval);

        // check number of running process
        inProcessing = run_queue.size() + sleepList.size() + ioWaitList.size();
        if(currentCpuTask->pid != -1){
            inProcessing++;
        }

    }
    fprintf(fp_memory, "page fault = %d\n", page_fault);

    // end!
    fclose(fp_sched);
    fclose(fp_memory);

}
*/