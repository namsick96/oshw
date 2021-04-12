#include <iostream>
#include <time.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>

using namespace std;

int N;
int *arr1;
int *arr2;
int process_num;

vector<pair<int, int> > index_pair(int &N, int &total_process_num)
{
    int divide_num = 1;
    while (1)
    {
        if (divide_num >= total_process_num)
        {
            break;
        }
        else
        {
            divide_num = divide_num * 2;
        }
    }

    int parsed_num;
    if (N % divide_num == 0)
    {
        parsed_num = N / divide_num;
    }
    else
    {
        parsed_num = (int)(N / divide_num) + 1;
    }
    //pair indicate start to end -1 idx is one set
    vector<pair<int, int> > vec_pair;
    int start = 0;
    int end = start + parsed_num;
    int count = 0;
    while (end < N && count < total_process_num - 1)
    {
        vec_pair.push_back(make_pair(start, end));
        start = end;
        end = end + parsed_num;
        count++;
    }
    vec_pair.push_back(make_pair(start, N));

    return vec_pair;
}

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
            dup2(fd[0], STDIN_FILENO);
            close(fd[1]);
            char *argv[] = {0};
            execvp("./program1", argv);
        }
        else if (pid < 0)
        {
            perror("fork error");
        }
        else
        {
            int tempdup = dup(STDOUT_FILENO);
            if (fd[1] != 1)
            {
                dup2(fd[1], STDOUT_FILENO);
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
            dup2(tempdup, STDOUT_FILENO); //rechange, roll back std table
            waitpid(pid, &status, WUNTRACED);
        }
    }

    return 0;
}
