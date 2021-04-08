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

using namespace std;
/*
  Function Declarations for builtin shell commands:
 */
int lsh_cd(char **args);
int lsh_help(char **args);
int lsh_exit(char **args);

/*
  List of builtin commands, followed by their corresponding functions.
 */
char *builtin_str[] = {
    "cd",
    "help",
    "exit"};

int (*builtin_func[])(char **) = {
    &lsh_cd,
    &lsh_help,
    &lsh_exit};

int lsh_num_builtins()
{
    return sizeof(builtin_str) / sizeof(char *);
}

/*
  Builtin function implementations.
*/

/**
   @brief Bultin command: change directory.
   @param args List of args.  args[0] is "cd".  args[1] is the directory.
   @return Always returns 1, to continue executing.
 */
int lsh_cd(char **args)
{
    if (args[1] == NULL)
    {
        fprintf(stderr, "lsh: expected argument to \"cd\"\n");
    }
    else
    {
        if (chdir(args[1]) != 0)
        {
            perror("lsh");
        }
    }
    return 1;
}

/**
   @brief Builtin command: print help.
   @param args List of args.  Not examined.
   @return Always returns 1, to continue executing.
 */
int lsh_help(char **args)
{
    int i;
    printf("Stephen Brennan's LSH\n");
    printf("Type program names and arguments, and hit enter.\n");
    printf("The following are built in:\n");

    for (i = 0; i < lsh_num_builtins(); i++)
    {
        printf("  %s\n", builtin_str[i]);
    }

    printf("Use the man command for information on other programs.\n");
    return 1;
}

/**
   @brief Builtin command: exit.
   @param args List of args.  Not examined.
   @return Always returns 0, to terminate execution.
 */
int lsh_exit(char **args)
{
    return 0;
}

/**
  @brief Launch a program and wait for it to terminate.
  @param args Null terminated list of arguments (including program).
  @return Always returns 1, to continue execution.
 */
int lsh_launch(char **args)
{
    pid_t pid;
    int status;
    char *envp[] = {(char *)"PATH=/bin", 0};

    pid = fork();
    if (pid == 0)
    {
        // Child process
        if (execve(args[0], args, envp) == -1)
        {
            perror("child process error");
        }
        exit(EXIT_FAILURE);
    }
    else if (pid < 0)
    {
        // Error forking
        perror("child processor did not made");
    }
    else
    {
        // Parent process
        do
        {
            waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;
}

/**
   @brief Execute shell built-in or launch program.
   @param args Null terminated list of arguments.
   @return 1 if the shell should continue running, 0 if it should terminate
 */
int lsh_execute(char **args)
{
    int i;

    if (args[0] == NULL)
    {
        // An empty command was entered.
        return 1;
    }

    for (i = 0; i < lsh_num_builtins(); i++)
    {
        if (strcmp(args[0], builtin_str[i]) == 0)
        {
            return (*builtin_func[i])(args);
        }
    }

    return lsh_launch(args);
}

/**
   @brief Read a line of input from stdin.
   @return The line from stdin.
 */
char *lsh_read_line(void) // 걍 스트링으로 하자..
{
    char *line = NULL;
    int bufsize = 1024; // have getline allocate a buffer for us
    if (cin.getline(line) == -1)
    {
        if (feof(stdin))
        {
            exit(EXIT_SUCCESS); // We received an EOF
        }
        else
        {
            perror("problem occur in reading line\n");
            exit(EXIT_FAILURE);
        }
    }
    return line;
}

#define LSH_TOK_BUFSIZE 64
#define LSH_TOK_DELIM " \t\r\n\a"
/**
   @brief Split a line into tokens (very naively).
   @param line The line.
   @return Null-terminated array of tokens.
 */
char **lsh_split_line(char *line)
{
    int bufsize = LSH_TOK_BUFSIZE, position = 0;
    char **tokens = malloc(bufsize * sizeof(char *));
    char *token, **tokens_backup;

    if (!tokens)
    {
        fprintf(stderr, "lsh: allocation error\n");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, LSH_TOK_DELIM);
    while (token != NULL)
    {
        tokens[position] = token;
        position++;

        if (position >= bufsize)
        {
            bufsize += LSH_TOK_BUFSIZE;
            tokens_backup = tokens;
            tokens = realloc(tokens, bufsize * sizeof(char *));
            if (!tokens)
            {
                free(tokens_backup);
                fprintf(stderr, "lsh: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, LSH_TOK_DELIM);
    }
    tokens[position] = NULL;
    return tokens;
}

int execute(char **args)

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

/**
   @brief Main entry point.
   @param argc Argument count.
   @param argv Argument vector.
   @return status code
 */
int main(int argc, char **argv)
{
    // Load config files, if any.

    // Run command loop.
    char *line; //변경 예정
    char **args;
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

        //check last word &
        last = line.substr(line.length() - 1, line.length());
        if (last == "&")
        {
            line = line.substr(0, line.length() - 1);
            bg_status = 1;
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

        //수정중..
        line = lsh_read_line();

        //이 부분은 main에서 돌린다. 그리고 마지막 명령어가 &이면 execute을 다른걸 준다.
        args = lsh_split_line(line);
        status = lsh_execute(args);

        delete (line);
        delete (args);
    } while (status);

    // Perform any shutdown/cleanup.

    return EXIT_SUCCESS;
}
