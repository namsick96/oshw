/*
#include "cmd_ioWait.h"
#include "cmd_sleep.h"
#include "cmd_malloc.h"
#include "cmd_memFree.h"
#include "cmd_lock.h"
#include "cmd_unlock.h"
#include "cmd_memAccess.h"
#include "PageBundle.h"
*/
#include "opIOwait.h"
#include "opSleep.h"
#include "Job.h"
#include <list>
#include <deque>
#include <vector>
#include <iostream>
#include "PhysicalMemoryTree.h"
#include "cmd_malloc.h"

#ifndef __COMMANDEXECUTE__
#define __COMMANDEXECUTE__

void commandExecute(Job *&currentCpuTask, list<Job *> &sleepList, list<Job *> &ioWaitList, list<Job *> &runningJobs_list, int currentCycle, PhysicalMemoryTree &physicalMemory, string page, int &page_fault); //, list<PageBundle *> *all_pages, PageBundle **physicalMem, string sched_opt, string page_opt, int &aid, int physicalNum, int currentCycle, int &page_fault);

#endif
