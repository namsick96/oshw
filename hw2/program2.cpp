#include <iostream>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>

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

int main(int argc, char *argv[])
{
    struct timespec begin, end;
    double tmpValue = 0.0;

    int total_process_num = atoi(argv[1]);
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

    scanf("%d", &N);
    arr1 = new int[N];
    arr2 = new int[N];

    for (int i = 0; i < N; i++)
    {
        cin >> arr1[i];
    }

    //나눠서 각각 그룹마다 돌리기
    //마지막 process에서는 남은거 다 돌리기 if ==마지막num 하면 for 남은거 쪼개서 넣기

    clock_gettime(CLOCK_MONOTONIC, &begin);
    partition(0, N - 1);
    clock_gettime(CLOCK_MONOTONIC, &end);

    for (int i = 0; i < N; i++)
    {
        cout << arr1[i] << " ";
    }
    cout << "\n";
    cout << ((end.tv_sec - begin.tv_sec) * 1000.0) + ((end.tv_nsec - begin.tv_nsec) / 1000000.0) << endl;

    return 0;
}
