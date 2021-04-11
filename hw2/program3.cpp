#include <iostream>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <vector>

using namespace std;

int N;
int *arr1;
int *arr2;
int process_num;

int main(int argc, char *argv[])
{
    struct timespec begin, end;
    double tmpValue = 0.0;

    int total_process_num = atoi(argv[1]);
    int divide_num = 1;

    scanf("%d", &N);
    arr1 = new int[N];
    arr2 = new int[N];

    vector<pair<int, int> > idx_pair = index_pair(N, total_process_num);

    for (int i = 0; i < N; i++)
    {
        cin >> arr1[i];
    }
    pid_t pid;
    int status;
    int fd[2];

    for (int i = 0; i < total_process_num; i++)
    {
        if (pipe(fd) == -1)
        {
            perror("pipe");
            exit(1);
        }

        pid = fork();
        if (pid == 0)
        {
            dup2(fd[0], 0);
            close(fd[1]);
            //close(fd[1]);
            char *argv[] = {0};
            execvp("./program1", argv);
        }
        else if (pid < 0)
        {
            perror("fork error");
        }
        else
        {
            int tempdup = dup(1);
            if (fd[1] != 1)
            {
                dup2(fd[1], 1);
            }
            close(fd[0]);
            int first = idx_pair[i].first;
            int second = idx_pair[i].second;
            int nums = second - first;

            cout << nums << endl;
            for (int k = first; k < second; k++)
            {
                cout << arr1[k] << endl;
            }
            dup2(tempdup, 1); //rechange, roll back std table
            waitpid(pid, &status, WUNTRACED);
        }
    }

    return 0;
}
