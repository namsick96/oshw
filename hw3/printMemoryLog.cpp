#include "printMemoryLog.h"

bool cmpPid(const pair<Job *, int> &a, const pair<Job *, int> &b)
{
    return a.second < b.second;
}

void travelNode(TreeNode *&current, vector<pair<int, int> > &v)
{
    if (current != nullptr)
    {
        if (!current->splited)
        {
            v.push_back(make_pair(current->frameNum, current->allocationID));
        }
        travelNode(current->left, v);
        travelNode(current->right, v);
    }
}

void printMemoryLog(FILE *out, Job *&currentCpuJob, deque<Job *> *run_queue, list<Job *> &sleepList, list<Job *> &ioWaitList, PhysicalMemoryTree &physicalMemory, string page, int currentCycle, int page_fault)
{
    int pid;
    int pageID;
    const char *name;
    int priority;
    int cycle = currentCycle;
    int opidx;
    int line;
    int op;
    int arg;

    pid = currentCpuJob->pid;
    pageID = currentCpuJob->pageTableFinalPageID - 1;
    name = currentCpuJob->file_name.c_str();
    priority = currentCpuJob->priority;

    //printf("memory start\n");

    if (currentCpuJob->pid != -1)
    {
        opidx = currentCpuJob->current_cmd_int - 1;
        line = opidx + 1;
        op = currentCpuJob->cmd_list[opidx].first;
        arg = currentCpuJob->cmd_list[opidx].second;
    }

    //line 1
    if (currentCpuJob->pid == -1)
    {
        fprintf(out, "[%d Cycle] Input : Function [NO-OP]\n", currentCycle);
    }
    else
    {
        if (op == 0)
        {
            int allocID = physicalMemory.physical_final_allocationID - 1;
            fprintf(out, "[%d Cycle] Input : Pid [%d] Function [ALLOCATION] Page ID [%d] Page Num[%d]\n", currentCycle, pid, pageID, arg);
        }
        else if (op == 1)
        {
            // memory access
            int pageCount = 0;
            for (int i = 0; i < currentCpuJob->pageTablePageNum; i++)
            {
                if (currentCpuJob->pageTable->pageIDv[i] == arg)
                {
                    pageCount++;
                }
            }
            fprintf(out, "[%d Cycle] Input : Pid [%d] Function [ACCESS] Page ID [%d] Page Num[%d]\n", currentCycle, pid, arg, pageCount);
        }
        else if (op == 2)
        {
            // memory release
            int pageCount = currentCpuJob->recentUsedPageNum;
            fprintf(out, "[%d Cycle] Input : Pid [%d] Function [RELEASE] Page ID [%d] Page Num[%d]\n", currentCycle, pid, arg, pageCount);
        }
        else if (op == 3)
        {
            fprintf(out, "[%d Cycle] Input : Pid [%d] Function [NONMEMORY]\n", currentCycle, pid);
        }
        else if (op == 4)
        {
            // Sleep
            fprintf(out, "[%d Cycle] Input : Pid [%d] Function [SLEEP]\n", currentCycle, pid);
        }
        else if (op == 5)
        {
            // IO Wait
            fprintf(out, "[%d Cycle] Input : Pid [%d] Function [IOWAIT]\n", currentCycle, pid);
        }
    }
    // line 2
    //여기서부터 하기
    //while문으로 dfs, splited==false인 것만 vector에 넣기. 이 벡터는 프린트 순서임.
    fprintf(out, "%-30s", ">> Physical Memory : ");
    int counter = 0;
    vector<pair<int, int> > v;
    vector<int> showlist;
    travelNode(physicalMemory.root, v);
    for (int k = 0; k < v.size(); k++)
    {
        int iternum = v[k].first;
        int allocid = v[k].second;
        for (int i = 0; i < iternum; i++)
        {
            showlist.push_back(allocid);
        }
    }
    for (int k = 0; k < showlist.size(); k++)
    {
        if (k % 4 == 0)
        {
            fprintf(out, "|");
        }
        if (showlist[k] == -1)
        {
            fprintf(out, "-");
        }
        else
        {
            fprintf(out, "%d", showlist[k]);
        }
    }
    fprintf(out, "|\n");

    //line3
    //runque, IOWAIT, SLEEP, curentJob까지 해서 전체 pid 순으로 정렬
    // 전체 다 돌면서 pair <Job*,int(pid)> 파악하기.
    vector<pair<Job *, int> > jobsSortedlist;
    jobsSortedlist.push_back(make_pair(currentCpuJob, currentCpuJob->pid));
    for (int i = 0; i < 10; i++)
    {
        if (!run_queue[i].empty())
        {
            for (int k = 0; k < run_queue[i].size(); k++)
            {
                jobsSortedlist.push_back(make_pair(run_queue[i][k], run_queue[i][k]->pid));
            }
        }
    }
    list<Job *>::iterator job_iter;
    for (job_iter = sleepList.begin(); job_iter != sleepList.end(); job_iter++)
    {
        Job *job = *job_iter;
        jobsSortedlist.push_back(make_pair(job, job->pid));
    }
    for (job_iter = ioWaitList.begin(); job_iter != ioWaitList.end(); job_iter++)
    {
        Job *job = *job_iter;
        jobsSortedlist.push_back(make_pair(job, job->pid));
    }
    sort(jobsSortedlist.begin(), jobsSortedlist.end(), cmpPid);

    deque<int> finishedPID;
    finishedPID.push_front(-2);
    for (int k = 0; k < jobsSortedlist.size(); k++)
    {
        if (jobsSortedlist[k].second == -1 or jobsSortedlist[k].second == finishedPID.front())
        {
            continue;
        }
        finishedPID.push_front(jobsSortedlist[k].second);
        Job *jobCase = jobsSortedlist[k].first;
        //subline1;
        fprintf(out, ">> pid(%d) %-20s", jobCase->pid, "Page Table(PID) : ");
        for (int i = 0; i < jobCase->pageTablePageNum; i++)
        {
            if (i % 4 == 0)
            {
                fprintf(out, "|");
            }
            if (jobCase->pageTable->pageIDv[i] == -1)
            {
                fprintf(out, "-");
            }
            else
            {
                fprintf(out, "%d", jobCase->pageTable->pageIDv[i]);
            }
        }
        fprintf(out, "|\n");

        //subline2;
        fprintf(out, ">> pid(%d) %-20s", jobCase->pid, "Page Table(AID) : ");
        for (int i = 0; i < jobCase->pageTablePageNum; i++)
        {
            if (i % 4 == 0)
            {
                fprintf(out, "|");
            }
            if (jobCase->pageTable->allocationIDv[i] == -1)
            {
                fprintf(out, "-");
            }
            else
            {
                fprintf(out, "%d", jobCase->pageTable->allocationIDv[i]);
            }
        }
        fprintf(out, "|\n");

        //subline3;
        fprintf(out, ">> pid(%d) %-20s", jobCase->pid, "Page Table(Valid) : ");

        for (int i = 0; i < jobCase->pageTablePageNum; i++)
        {
            if (i % 4 == 0)
            {
                fprintf(out, "|");
            }
            if (jobCase->pageTable->validBitv[i] == -1)
            {
                fprintf(out, "-");
            }
            else
            {
                fprintf(out, "%d", jobCase->pageTable->validBitv[i]);
            }
        }
        fprintf(out, "|\n");

        //subline4
        fprintf(out, ">> pid(%d) %-20s", jobCase->pid, "Page Table(Ref) : ");
        for (int i = 0; i < jobCase->pageTablePageNum; i++)
        {
            if (i % 4 == 0)
            {
                fprintf(out, "|");
            }
            if (strcmp(page.c_str(), "lru") == 0)
            {
                fprintf(out, "-");
            }
            else
            {
                if (jobCase->pageTable->pageIDv[i] == -1)
                {
                    fprintf(out, "-");
                }
                else
                {
                    fprintf(out, "%d", jobCase->pageTable->validBitv[i]);
                }
            }
        }
        fprintf(out, "|\n");
    }
    fprintf(out, "\n");
}