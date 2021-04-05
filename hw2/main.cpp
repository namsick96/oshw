#include <pthread.h>
#include <cstdio>
#include <unistd.h>
#include <stdlib.h>
using namespace std;

int main(int argc, char **argv)
{
    pid_t pid;

    pid = fork();
    if (pid == 0)
    {
        printf("자식 프로세스\n");
        int execvp(const char *file, char *const argv[]);
        // exit(0);
    }
    if (pid > 0)
    {
        printf("Wait\n");
        //wait(NULL);
        printf("Exit\n");
    }
    return 0;
}