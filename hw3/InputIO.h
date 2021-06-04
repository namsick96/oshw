#include <string>
#include <cstring>
#ifndef __INPUTIO__
#define __INPUTIO__

using namespace std;

class InputIO
{

public:
    int target_pid;
    int startCycle;

    InputIO(int start, int pid_num)
    {
        startCycle = start;
        target_pid = pid_num;
    }

    InputIO()
    {
    }
};

#endif //PRACTICE1_INPUTIO_H
