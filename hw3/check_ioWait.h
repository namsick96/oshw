#include <list>
#include <queue>
#include <vector>
#include "Job.h"
#include "InputIO.h"

#ifndef __CHECK_IOWAIT__
#define __CHECK_IOWAIT__

void check_ioWait(queue<Job *> *run_queue, list<Job *> &ioWaitList, vector<InputIO *> &input_list, int currentCycle);

#endif