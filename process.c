//
// Created by kunal on 2/9/22.
//

#include "header.h"
#include "builtin.h"
#include "process.h"
#include "process_functions.h"
#include "jobs.h"

int timespan = 0;

//launch a process given by the tokenized inpt
int launchProcess(char** inpt)
{
    //check whether & is given for background
    int backG = 0;
    int ch = '&';
    char * ptr;

    // checks if & is the last argument. e.g. sleep 10 &
    if(strcmp(inpt[getArgc(inpt)], "&")==0)
    {
        free(inpt[getArgc(inpt)]);
        inpt[getArgc(inpt)] = NULL;
        backG = 1;
    }

    // checks if & is present in the last argument. e.g. sleep 10&
    if(strchr(inpt[getArgc(inpt)], ch)!=NULL)
    {
        ptr = strrchr( inpt[getArgc(inpt)], ch ); // The  strchr() function returns a pointer to the first occurrence of the character c in the string s.
        ptr[0] = '\0';
        backG = 1;
    }

    //fork process
    int st;
    pid_t pID;
    pID = fork(); // Clone the calling process, creating an exact copy.

    // add proc to list
    addProc(pID, inpt);

    //executes in child process
    if(pID==0)
    {
        setpgid(pID, pID);
        signal(SIGQUIT, SIG_DFL);
        signal(SIGINT, SIG_DFL);
        signal(SIGCHLD, SIG_DFL);
        signal(SIGTSTP, SIG_DFL);
        signal(SIGTTIN, SIG_DFL);
        signal(SIGTTOU, SIG_DFL);
        execvp(inpt[0], inpt);
        perror("Unrecognised executable\n");
        _exit(1);
    }
        //fork error
    else if (pID<0)
    {
        removeProc(pID);
        perror("Could not fork process\n");
        return 0;
    }
        //in parent fork successful
    else
    {
        setpgid(pID, pID); // Set the process group ID of the process matching PID to PGID.
        //not background
        if(!backG)
        {
            do
            {
                time_t begin = time(NULL);
                waitpid(pID, &st, WUNTRACED);
                time_t end = time(NULL);
                timespan = (end - begin);
            }while(!WIFEXITED(st));
            return timespan;
        }
            //background
        else
        {
            job_count++;
            printf("[%lld] %d %s\n", job_count, pID, inpt[0]); // DETAILS OF BG PROCESS
            job_arr[job_count].pid = pID;
            strcpy(job_arr[job_count].name, inpt[0]); // TO KILL PROCESSES
            return 0;
        }

    }
}

//display process info
void pinfo(long long no_of_arg, char arg[][200])
{
    char id[50];
    char path[2000];
    char filepath[2000];
    if (no_of_arg > 1) //for other processes
    {
        sprintf(path, "/proc/%s/stat", arg[1]);
        sprintf(filepath, "/proc/%s/exe", arg[1]);
        strcpy(id, arg[1]);
    }
    else // for shell
    {

        sprintf(id, "%d", getpid());
        sprintf(path, "/proc/%s/stat", id);
        sprintf(filepath, "/proc/%s/exe", id);
    }

    //strcpy(filepath, path);
    FILE *stat_file;
    stat_file = fopen(path, "r"); // Open a file and create a new stream for it.
    if (stat_file == NULL)
    {
        printf("ERROR: The process does not exist\n");
        return;
    }
    else
    {
        if (no_of_arg > 1)
        {
            printf("pid : %s\n", arg[1]);
        }
        else
        {
            printf("pid : %s\n", id);
        }

        // get the status , memory size and id's if it's running in fore/back-ground
        char buff[2000], status[5], memory[100], ppid[1000], pgrp[1000];
        long long i;
        for (i = 1; fscanf(stat_file, "%s", buff) == 1; i++) // Read formatted input from stat_file.
        {
            if (i == 3)
            {
                strcpy(status, buff);
            }
            if (i == 23)
            {
                strcpy(memory, buff);
            }
            if (i == 5)
            {
                strcpy(ppid, buff);
            }
            if (i == 8)
            {
                strcpy(pgrp, buff);
            }
        }
        fclose(stat_file); // close stat_file
        printf("Process Status : %s", status);
        if ((strcmp(ppid, pgrp) == 0) && (strcmp(status, "S") == 0 || strcmp(status, "R") == 0))
        {
            printf("+\n");
        }
        else
        {
            printf("\n");
        }
        printf("memory : %s {Virtual Memory}\n", memory);

        //finding and printing executable path
        char exec_path[2000];
        int len;
        len = readlink(filepath, exec_path, 2000); // Read the contents of the symbolic link filepath into no more than 2000 bytes of exec_path.
        if (len == -1)
        {
            printf("Cannot find executable path for pid: %s\n", id);
            return;
        }
        exec_path[len] = '\0';

        char *pointer = strstr(exec_path, homeDir);
        if (pointer != NULL)
        {
            char temp[2000];
            sprintf(temp, "%s%s", "~", pointer + strlen(homeDir));
            strcpy(exec_path, temp);
        }
        printf("Executable Path : %s\n", exec_path);
    }
}