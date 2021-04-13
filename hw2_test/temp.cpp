#include <iostream>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>
#include <vector>
#include <mutex>
#include <pthread.h>

using namespace std;

mutex syn; //for synchronization

int N;
int *arr1;
int *arr2;

struct ThreadArgs
{
    int left;
    int right;
    int th_num;
};

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
    {
        arr2[k++] = arr1[tmp++];
    }

    for (int i = left; i <= right; i++)
    {
        arr1[i] = arr2[i];
    }
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

void *partitionOne(void *data)
{

    ThreadArgs *threadArgs = (ThreadArgs *)data;
    int left = threadArgs->left;
    int right = threadArgs->right;

    syn.lock();
    cout << left << " " << right << endl;
    //syn.unlock();
    struct timespec begin, end;
    clock_gettime(CLOCK_MONOTONIC, &begin);
    int mid;
    if (left < right)
    {
        mid = (left + right) / 2;
        partition(left, mid);
        partition(mid + 1, right);
        merge(left, right);
    }
    clock_gettime(CLOCK_MONOTONIC, &end);

    //syn.lock();
    for (int i = left; i < right; i++)
    {
        cout << arr2[i] << " ";
    }
    cout << endl;
    cout << ((end.tv_sec - begin.tv_sec) * 1000.0) + ((end.tv_nsec - begin.tv_nsec) / 1000000.0) << endl;
    syn.unlock();
    pthread_exit(NULL);
}

vector<pair<int, int> > index_pair(int &N, int &total_thread_num)
{
    int divide_num = 1;
    while (1)
    {
        if (divide_num >= total_thread_num)
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
    while (end < N && count < total_thread_num - 1)
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

    int total_thread_num = atoi(argv[1]);
    int divide_num = 1;

    scanf("%d", &N);
    arr1 = new int[N];
    arr2 = new int[N];

    vector<pair<int, int> > idx_pair = index_pair(N, total_thread_num);

    for (int i = 0; i < N; i++)
    {
        cin >> arr1[i];
    }

    for (int i = 0; i < N; i++)
    {
        cout << arr1[i] << " ";
    }

    pthread_t pthread[total_thread_num];
    long thread_times_ms[total_thread_num];

    clock_gettime(CLOCK_MONOTONIC, &begin);

    for (int i = 0; i < total_thread_num; i++)
    {
        ThreadArgs *threadArgs;
        threadArgs = new ThreadArgs();
        threadArgs->left = idx_pair[i].first;
        threadArgs->right = idx_pair[i].second;

        int thread_id = pthread_create(&pthread[i], NULL, partitionOne, (void *)threadArgs);

        if (thread_id < 0)
        {
            perror("Fail to Thread create");
            exit(0);
        }
    }

    int status;
    for (int i = 0; i < total_thread_num; i++)
    {
        int status;
        pthread_join(pthread[i], (void **)&status);
    }
    //test whole
    //maybe sorted in group
    for (int i = 0; i < N; i++)
    {
        cout << arr2[i] << " ";
    }

    return 0;
}
