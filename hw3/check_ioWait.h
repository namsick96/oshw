#include <list>
#include <queue>
#include <deque>
#include <vector>
#include "Job.h"
#include "InputIO.h"

#ifndef __CHECK_IOWAIT__
#define __CHECK_IOWAIT__

void check_ioWait(deque<Job *> *run_queue, list<Job *> &ioWaitList, vector<InputIO *> &input_list, int currentCycle);

#endif