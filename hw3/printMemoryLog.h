#include <list>
#include <vector>
#include <deque>
#include <stdio.h>
#include <algorithm>
#include "Job.h"
#include "PhysicalMemoryTree.h"

#ifndef __PRINTMEMORYLOG__
#define __PRINTMEMORYLOG__

void printMemoryLog(FILE *out, Job *&currentCpuJob, deque<Job *> *run_queue, list<Job *> &sleepList, list<Job *> &ioWaitList, PhysicalMemoryTree &physicalMemory, string page, int currentCycle, int page_fault);
void travelNode(TreeNode *&current, vector<pair<int, int> > &v);
bool cmpPid(const pair<Job *, int> &a, const pair<Job *, int> &b);

#endif
