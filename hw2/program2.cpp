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

void merge(int data[], int left, int right)
{
    int temp[right + 1];
    int mid = (left + right) / 2;
    int i = left;
    int j = mid + 1;
    int k = left;
    while (i <= mid && j <= right)
    {
        if (data[i] > data[j])
            temp[k++] = data[i++];
        else
            temp[k++] = data[j++];
    }
    int tmp;
    if (i > mid)
    {
        tmp = j;
    }
    else
    {
        tmp = i;
    }
    while (k <= right)
    {
        temp[k++] = data[tmp++];
    }

    for (int i = left; i <= right; i++)
    {
        data[i] = temp[i];
    }
}

void partition(int data[], int left, int right)
{
    int mid;
    if (left < right)
    {
        mid = (left + right) / 2;
        partition(data, left, mid);
        partition(data, mid + 1, right);

        merge(data, left, right);
    }
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
    int fd2[2];

    clock_gettime(CLOCK_MONOTONIC, &begin);
    for (int i = 0; i < total_process_num; i++)
    {
        if (pipe(fd) == -1)
        {
            perror("pipe");
            exit(1);
        }
        if (pipe(fd2) == -1)
        {
            perror("pipe");
            exit(1);
        }
        pipe(fd2);

        pid = fork();
        if (pid == 0)
        {
            //child
            close(fd[1]);
            close(fd2[0]);
            dup2(fd[0], STDIN_FILENO);
            dup2(fd2[1], STDOUT_FILENO);

            string temp2 = "program2";
            char *temp = new char[temp2.size() + 1];
            copy(temp2.begin(), temp2.end(), temp);
            temp[temp2.size()] = '\0';
            //strcpy(temp, temp2.c_str());
            char *argv[] = {temp, 0};

            //char *argv[] = {0};
            execvp("./program1", argv);
        }
        else if (pid < 0)
        {
            perror("fork error");
        }
        else
        {
            close(fd[0]);
            close(fd2[1]);
            int tempdup = dup(STDOUT_FILENO);
            if (fd[1] != 1)
            {
                dup2(fd[1], STDOUT_FILENO);
            }
            int tempdup2 = dup(STDIN_FILENO);
            if (fd2[0] != 0)
            {
                dup2(fd2[0], STDIN_FILENO);
            }
            int first = idx_pair[i].first;
            int second = idx_pair[i].second;
            int nums = second - first;

            cout << nums << endl;
            for (int k = first; k < second; k++)
            {
                cout << arr1[k] << endl;
            }
            for (int k = first; k < second; k++)
            {
                cin >> arr2[k];
            }
            dup2(tempdup2, STDIN_FILENO);
            dup2(tempdup, STDOUT_FILENO); //rechange, roll back std table
            waitpid(pid, &status, WUNTRACED);
            /*
            this is waitpid in version. this does not get child process output
            waitpid(pid, &status, WUNTRACED);
            for (int k = first; k < second + 1; k++)
            {
                //int temp;
                cin >> arr2[k];
                //cin >> temp;
                //cout << temp << " ";
            }
            dup2(tempdup2, STDIN_FILENO);
            dup2(tempdup, STDOUT_FILENO);
            */
        }
    }
    /*
    for (int i = 0; i < total_process_num; i++)
    {
        waitpid(pid, &status, WUNTRACED);
    }
    */

    //final sort
    partition(arr2, 0, N - 1);
    clock_gettime(CLOCK_MONOTONIC, &end);
    for (int k = 0; k < N; k++)
    {
        cout << arr2[k] << " ";
    }
    cout << "\n";
    cout << ((end.tv_sec - begin.tv_sec) * 1000.0) + ((end.tv_nsec - begin.tv_nsec) / 1000000.0) << endl;

    return 0;
}
