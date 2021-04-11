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
#include <vector>
#include <fcntl.h>

/* 
made by namsick96
Dev Date 210408
*/

using namespace std;

int change_dir(char *args[])
{
    if (args[1] == NULL)
    {
        fprintf(stderr, " No argument for \"cd\"\n");
    }
    else
    {
        if (chdir(args[1]) != 0)
        {
            perror("changing error");
        }
    }
    return 1;
}

int launch(char *args[], int bg, int input_status, int output_status, string input_file, string output_file)
{
    pid_t pid;
    int status;

    pid = fork();
    if (pid == 0)
    {
        char *envp[] = {(char *)"PATH=/bin", 0};
        int in, out;

        if (input_status)
        {
            in = open(input_file.c_str(), O_RDONLY);
            dup2(in, 0);
        }
        if (output_status)
        {
            out = open(output_file.c_str(), O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
            dup2(out, 1);
        }
        if (execvp(args[0], args) == -1)
        {
            perror("child process error");
        }
        if (input_status)
        {
            close(in);
        }
        if (output_status)
        {
            close(out);
        }
        exit(EXIT_FAILURE);
    }
    else if (pid < 0)
    {
        perror("child processor did not made");
    }
    else
    {
        //parent process
        if (bg)
        {
            //not wait implement
            return 1;
        }
        else
        {
            do
            {
                waitpid(pid, &status, WUNTRACED);
            } while (!WIFEXITED(status) && !WIFSIGNALED(status));
        }
    }
    return 1;
}

int execute(char *args[], int bg, int input_status, int output_status, string input_file, string output_file)
{
    if (args[0] == NULL)
    {
        // An empty command was entered.
        return 1;
    }
    if (strcmp(args[0], "exit") == 0)
    {
        return 0;
    }
    if (strcmp(args[0], "cd") == 0)
    {

        return change_dir(args);
    }
    return launch(args, bg, input_status, output_status, input_file, output_file);
}

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
    // Run command loop.
    int status;

    do
    {
        /* 
        start display
        */
        display_prompt();

        /*
        initialize status
        bg_status for checking last &
        */
        int bg_status = 0;

        //get line from user
        string line;
        string last;

        getline(cin, line);
        // check last word if size is bigger than 1
        if (line.size() > 0)
        {
            last = line.substr(line.length() - 1, line.length());
            if (last == "&")
            {
                line = line.substr(0, line.length() - 1);
                bg_status = 1;
            }
        }

        /*
        > , < part check
        input_status, output_status
        filenames: inputfile, outputfile
        ans discard it from user input "line"
        */
        int input_status = 0;
        int output_status = 0;
        string inputfile;
        string outputfile;

        int inputIdx = line.find("<");
        int outputIdx = line.find(">");

        // no > <
        if (inputIdx == -1 && outputIdx == -1)
        {
            //pass
        }

        //inputfile occur <
        else if (inputIdx != -1 && outputIdx == -1)
        {
            input_status = 1;

            int inFileNameEndIdx = line.find(" ", inputIdx);
            if (inFileNameEndIdx == inputIdx + 1)
            {
                inFileNameEndIdx = line.find(" ", inFileNameEndIdx + 1);
            }

            inputfile = line.substr(inputIdx + 1, inFileNameEndIdx - inputIdx - 1);            //file part
            inputfile.erase(remove(inputfile.begin(), inputfile.end(), ' '), inputfile.end()); //file part blank erase

            line.erase(inputIdx, inFileNameEndIdx - inputIdx + 1); //file part eliminate from line
        }
        //outputfile occur >
        else if (inputIdx == -1 && outputIdx != -1)
        {
            output_status = 1;

            int outFileNameEndIdx = line.find(" ", outputIdx);
            if (outFileNameEndIdx == outputIdx + 1)
            {
                outFileNameEndIdx = line.find(" ", outFileNameEndIdx + 1);
            }

            outputfile = line.substr(outputIdx + 1, outFileNameEndIdx - outputIdx - 1);            //file part
            outputfile.erase(remove(outputfile.begin(), outputfile.end(), ' '), outputfile.end()); //file part blank erase

            line.erase(outputIdx, outFileNameEndIdx - outputIdx + 1); //file part eliminate from line
        }

        // both are occur  >  <
        else if (inputIdx != -1 && outputIdx != -1)
        {
            output_status = 1;

            int outFileNameEndIdx = line.find(" ", outputIdx);
            if (outFileNameEndIdx == outputIdx + 1)
            {
                outFileNameEndIdx = line.find(" ", outFileNameEndIdx + 1);
            }

            outputfile = line.substr(outputIdx + 1, outFileNameEndIdx - outputIdx - 1);            //file part
            outputfile.erase(remove(outputfile.begin(), outputfile.end(), ' '), outputfile.end()); //file part blank erase

            line.erase(outputIdx, outFileNameEndIdx - outputIdx + 1); //file part eliminate from line

            //line has changed due to erase, so get < index again
            inputIdx = line.find("<");
            input_status = 1;

            int inFileNameEndIdx = line.find(" ", inputIdx);
            if (inFileNameEndIdx == inputIdx + 1)
            {
                inFileNameEndIdx = line.find(" ", inFileNameEndIdx + 1);
            }

            inputfile = line.substr(inputIdx + 1, inFileNameEndIdx - inputIdx - 1);            //file part
            inputfile.erase(remove(inputfile.begin(), inputfile.end(), ' '), inputfile.end()); //file part blank erase

            line.erase(inputIdx, inFileNameEndIdx - inputIdx + 1); //file part eliminate from line
        }
        /*
        arguments parsing part
        line was trimmed and only argument left so make arguments split and make it suitable for exec
        */
        //arguments parsing
        string token1;
        stringstream streamline(line);
        vector<string> tokens;
        while (getline(streamline, token1, ' '))
        {
            tokens.push_back(token1);
        }
        char *args[tokens.size() + 1];
        for (int i = 0; i < tokens.size(); i++)
        {
            char *name = new char[tokens[i].size() + 1];
            copy(tokens[i].begin(), tokens[i].end(), name);
            name[tokens[i].size()] = '\0';
            args[i] = name;
        }
        args[tokens.size()] = 0;

        //execute!
        status = execute(args, bg_status, input_status, output_status, inputfile, outputfile);

    } while (status);

    // Perform any shutdown/cleanup.

    return EXIT_SUCCESS;
}
