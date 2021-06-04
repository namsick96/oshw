#include "Job.h"
#include <list>
#include <vector>
#include <queue>

#ifndef PRACTICE1_REPORT_CRITERIA_H
#define PRACTICE1_REPORT_CRITERIA_H

using namespace std;

void report_criteria(Task *&currentCpuTask, list<Task *> *run_queue, int currentCycle);

#endif //PRACTICE1_REPORT_CRITERIA_H