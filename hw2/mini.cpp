#include <pthread.h>
#include <cstdio>
#include <unistd.h>
#include <stdlib.h>
#include <csignal>
#include <cstring>
#include <string>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/resource.h>
#include <time.h>
#include <iostream>
#include <sstream>

using namespace std;
/* 
made by namsick96
Dev Date 210406
*/

void display_prompt()
{
    //time output
    time_t curTime = time(NULL);
    struct tm *pLocal = NULL;
    pLocal = localtime(&curTime);
    printf("[%02d:%02d:%02d]", pLocal->tm_hour, pLocal->tm_min, pLocal->tm_sec);
    //user
    printf("%s@", getenv("USER"));

    //pwd
    char nowdir[256];
    getcwd(nowdir, 256);
    cout << nowdir << "$";
}

int main(int argc, char **argv)
{
    int i;
    pid_t pid; //child process pid

    while (1)
    {
        //show first
        string command = "\n";
        while (command == "\n")
        {
            display_prompt();
            cin >> command;
            if (command == "exit")
            {
                return 0;
            }
        }
        string argments;
        getline(cin, argments);
        //command
        string pathline = "/bin/" + command;
        cout << pathline << endl;
        cout << argments;
        char *args[argments.size()];

        // // fork
        // pid = fork();
        // if (pid == 0)
        // {
        //     execv(pathline.c_str(), args)
        // }
        // else
        // {
        // }
        // break;
    }
    return 0;
}