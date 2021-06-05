#include "Job.h"
#include <list>
#include <vector>
#include <queue>
#include <deque>

#ifndef __status_update__
#define __status_update__

using namespace std;

void status_update(Job *&currentCpuTask, deque<Job *> *run_queue, int currentCycle);

#endif //status_update