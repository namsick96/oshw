#include <pthread.h>
#include <cstdio>
#include <unistd.h>
#include <stdlib.h>
#include <csignal>
#include <cstring>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/resource.h>
#include <time.h>

using namespace std;
/* 
made by namsick96
Dev Date 210406
*/

int main(int argc, char **argv)
{
    int i;      //루프를 위한 변수
    int status; //자식 프로세스가 종료 될 때 상태 값을 받기 위한 변수

    struct timeval start_command, end_command;                 //command가 실행되고 경과된 시간을 측정하기 위한 자료형
    struct timeval start_UCPU, start_SCPU, end_UCPU, end_SCPU; //사용된 CPU시간을 측정하기 위한 자료형
    struct rusage usage;                                       //프로세스의 상태 값을 받기 위한 자료형
    int first_CPU_giveup, last_CPU_giveup;                     //프로세스가 자발적으로 CPU점유를 포기한 횟수를 세기위한 변수
    int first_CPU_pass, last_CPU_pass;                         //프로세스가 선점된 횟수를 세기위한 변수

    char *new_argv[500]; //수정한 **argv를 저장하기 위한 배열
    char _argv[500];     //수정한 *argv를 저장하기 위한 배열

    int new_argc; //수정한 argc를 저장하기 위한 변수

    char *cut_path;      //PATH 경로를 parsing 하고 그 결과를 받는 포인터
    char copy_path[500]; //PATH 경로를 받아오는 문자형 배열
    char new_path[500];  //PATH 경로를 수정하여 저장할 문자형 배열
    pid_t pid;           //자식프로세스의 ID 값을 저장할 변수

    char command[100]; //USER의 명령어를 저장할 문자형 배열

    new_argc = argc; //무한 루프를 돌기 전 받아온 argc를 저장
    //무한 루프 시작. 종료는 exit 명령어만 가능
    while (1)
    {
        //command line에 명령어가 존재하지 않을 경우
        if (new_argc == 1)
        {                                       //만약 명령어가 없으면 쉘처럼 동작함
            fgets(_argv, sizeof(_argv), stdin); //command를 받음
            new_argv[0] = strtok(_argv, " ");   //command를 파싱

            //command line 에서 아규먼트를 파싱
            for (i = 1;; i++)
            {
                if (!(new_argv[i] = strtok(NULL, " ")))
                    break;
                new_argc++;
            }
            //마지막 개행문자 제거
            new_argv[new_argc - 1] = strtok(new_argv[new_argc - 1], "\n");
            //마지막 글자 뒤에 NULL 포인터 입력
            new_argv[new_argc] = (char *)NULL;
            strcpy(command, new_argv[0]);     //command를 복사
            if (strcmp(command, "exit") == 0) //만약 coommand가 exit명령어 이면 프로그램은 종료됨
                return 0;
        }
        //command lined에 command가 존재할 경우
        else
        {
            //받아온 command와 아규먼트를 새로운 배열에 복사
            for (i = 0; i < new_argc; i++)
            {
                if (i == new_argc - 1)
                    new_argv[i] = (char *)NULL;
                else
                    new_argv[i] = argv[i + 1];
            }
            strcpy(command, argv[1]); //command 복사
        }

        //PATH를 파싱
        strcpy(copy_path, getenv("PATH"));
        cut_path = strtok(copy_path, ":");
        strcpy(new_path, cut_path);
        strcat(new_path, "/");
        strcat(new_path, command); //파싱한 PATH뒤에 명령어를 붙임
        strcat(new_path, "");
        //자식 프로세스 생성
        pid = fork();

        //프로세스 통계 출력을 위해
        gettimeofday(&start_command, NULL); //command가 실행된 시간
        getrusage(RUSAGE_CHILDREN, &usage); //자식프로세스의 상태
        start_UCPU = usage.ru_utime;        //USER CPU시간
        start_SCPU = usage.ru_stime;        //SYSTEM CPU시간
        first_CPU_giveup = usage.ru_nvcsw;  //프로세스가 자발적으로 CPU를 포기한 횟수
        first_CPU_pass = usage.ru_nivcsw;   //프로세스가 선점된 횟수

        //프로세스가 실패할 경우
        if (pid == -1)
        {
            printf("fork 실패 프로세스 \n");
            return -1;
        }

        if (pid == 0)
        { //자식프로세스
            //PATH가 맞을 때 까지 무한 루프, 모든 PATH 실패할 경우 탈출
            while (execv(new_path, new_argv) == -1)
            {
                //PATH를 파싱
                if (!(cut_path = strtok(NULL, ":")))
                    break;
                //파싱 후 PATH 뒤에 경로를 붙임
                strcpy(new_path, cut_path);
                strcat(new_path, "/");
                strcat(new_path, command);
                strcat(new_path, "");
            }
            //자식프로세스가 명령을 수행하면 종료됨
            exit(0);
        }

        else
        { //부모프로세스

            waitpid(-1, &status, 0); //자식 프로세스가 끝날때 까지 기다림
            //프로세스 통계를 위해 값을 받음
            gettimeofday(&end_command, NULL);   //command 실행 후 경과된 시간
            getrusage(RUSAGE_CHILDREN, &usage); //자식프로세스 상태
            end_SCPU = usage.ru_stime;          //SYSTEM CPU 시간
            end_UCPU = usage.ru_utime;          //USER CPU 시간
            last_CPU_giveup = usage.ru_nvcsw;   //프로세스가 자발적으로 CPU를 포기한 횟수
            last_CPU_pass = usage.ru_nivcsw;    //프로세스가 선점된 횟수

            //만약 밀리세크가 시작시간이 끝시간보다 클 경우
            //끝시간의 초단위 시간을 1초 빼고 밀리세크를 1초만큼 증가

            if (end_SCPU.tv_usec <= start_SCPU.tv_usec)
            {
                if (end_SCPU.tv_usec == start_SCPU.tv_usec && end_SCPU.tv_sec == start_SCPU.tv_sec)
                    ;
                else
                {
                    end_SCPU.tv_sec--;
                    end_SCPU.tv_usec += 1000000;
                }
            }

            if (end_UCPU.tv_usec <= start_UCPU.tv_usec)
            {
                if (end_UCPU.tv_usec == start_UCPU.tv_usec && end_UCPU.tv_sec == start_UCPU.tv_sec)
                    ;
                else
                {
                    end_UCPU.tv_sec--;
                    end_UCPU.tv_usec += 1000000;
                }
            }

            if (end_command.tv_usec <= start_command.tv_usec)
            {
                if (end_command.tv_usec == start_command.tv_usec && end_command.tv_sec == start_command.tv_sec)
                    ;
                else
                {
                    end_command.tv_sec--;
                    end_command.tv_usec += 1000000;
                }
            }
            //프로세스 통계 출력
            printf("사용된 USER CPU 시간 : %d.%06dsec\n", end_UCPU.tv_sec - start_UCPU.tv_sec, end_UCPU.tv_usec - start_UCPU.tv_usec);
            printf("사용된 SYSTEM CPU 시간 : %d.%06dsec\n", end_SCPU.tv_sec - start_SCPU.tv_sec, end_SCPU.tv_usec - start_SCPU.tv_usec);
            printf("command가 실행되고 경가된 시간 : %d.%06dsec\n", end_command.tv_sec - start_command.tv_sec, end_command.tv_usec - start_command.tv_usec);
            printf("프로세스가 선점된 횟수 : %d\n", last_CPU_pass - first_CPU_pass);
            printf("프로세스가 자발적로 cpu 점유를 포기한 회수 : %d\n", last_CPU_giveup - first_CPU_giveup);
        }
        //쉘로 다시 만들어주기 위해 다시 COMMAND LINE을 받도록 다시 수정
        new_argc = 1;
    }
}