#include <vector>
#include <list>
#include <queue>
#include <deque>
#include "Job.h"

#ifndef __CHECK_SLEEP__
#define __CHECK_SLEEP__

void check_sleep(deque<Job *> *run_queue, list<Job *> &sleepList);

#endif //PRACTICE1_CHECK_SLEEP_H
