#include "Job.h"
/*
void Job::setData(int vmemSize, int pageSize)
{

    pageTableSize = vmemSize / pageSize;

    pageTable_aid = new PageBundle *[pageTableSize];
    pageTable_valid = new PageBundle *[pageTableSize];

    for (int i = 0; i < pageTableSize; i++)
    {
        if (i == 0)
        {
            pageTable_aid[0] = new PageBundle(pageTableSize, -1, 0);
            pageTable_valid[0] = new PageBundle(pageTableSize, -1, 0);
        }
        else
        {
            pageTable_aid[i] = nullptr;
            pageTable_valid[i] = nullptr;
        }
    }

    ifstream process;
    string process_name = directory + "/" + file_name;
    process.open(process_name.c_str());

    process >> cmd_num;
    for (int i = 0; i < cmd_num; i++)
    {
        int *cmd = new int[2];
        for (int k = 0; k < 2; k++)
        {
            process >> cmd[k];
        }
        cmd_list.push_back(cmd);
    }
    process.close();

    current_cmd = cmd_list.begin();
    current_cmd_int = 1;
}
*/
void Job::printCmd()
{
    printf("%d  %d  %s\n", pid, cmd_num, file_name.c_str());
    vector<int *>::iterator iter = cmd_list.begin();
    for (; iter != cmd_list.end(); iter++)
    {
        int *cmd = *iter;
        printf("cmd : %d   | opt : %d\n", cmd[0], cmd[1]);
    }
}
