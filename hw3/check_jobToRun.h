#include <vector>
#include <list>
#include <queue>
#include <deque>
#include "Job.h"

#ifndef __CHECK_JOB_TORUN__
#define __CHECK_JOB_TORUN__

void check_jobToRun(deque<Job *> *run_queue, list<Job *> &runningJobs_list, vector<Job *> &jobs_list, int currentCycle);

#endif //PRACTICE1_CHECK_TASKTORUN_H
