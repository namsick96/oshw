#include <iostream>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <vector>
#include <mutex>
#include <pthread.h>

using namespace std;

int N;
int *arr1;
int *arr2;
int process_num;

void merge(int left, int right)
{
    int mid = (left + right) / 2;

    int i = left;
    int j = mid + 1;
    int k = left;
    while (i <= mid && j <= right)
    {
        if (arr1[i] > arr1[j])
            arr2[k++] = arr1[i++];
        else
            arr2[k++] = arr1[j++];
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
        arr2[k++] = arr1[tmp++];

    for (int i = left; i <= right; i++)
        arr1[i] = arr2[i];
}

void partition(int left, int right)
{
    int mid;
    if (left < right)
    {
        mid = (left + right) / 2;
        partition(left, mid);
        partition(mid + 1, right);
        merge(left, right);
    }
}

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

    return 0;
}
