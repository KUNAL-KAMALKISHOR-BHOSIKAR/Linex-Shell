//
// Created by kunal on 25/9/22.
//

#include "header.h"
#include "display.h"
#include "builtin.h"
#include "process.h"
#include "process_functions.h"
#include "jobs.h"

struct proc
{
    int number;
    pid_t pid;
    char name[200];
    char status[20];
};

struct proc** procList = NULL;

//gives the executable name for a given PID
void pidToName(int pID, char* name)
{
    //check if pid is in procList
    for(int i = 0; procList[i] != NULL; i++)
        if(procList[i]->pid == pID)
        {
            strcpy(name, procList[i]->name);
            return;
        }

    //if pid not in procList get from /proc/pid/comm
    sprintf(name, "/proc/%d/comm", pID);
    int f;
    f = open(name, O_RDONLY); // The open() system call opens the file specified by pathname.
    if(f != -1)
    {
        int charsRead = read(f, name, MAX_TOK_SIZE); // Read MAX_TOK_SIZE into name from f.
        if(charsRead>0)
        {
            name[charsRead-1]='\0';
        }
        else
        {
            strcpy(name, "unknown");
        }
        close(f); // Close the file descriptor f.
    }
    return;
}

//change status of a proc to 0-running 1-stopped 3-foreground
void editProc(int pid, int status)
{
    if(procList != NULL)
    {
        //if pid exists in the procList update its status
        for(int i = 0; procList[i] != NULL; i++)
        {
            if(pid == procList[i]->pid)
                if(status == 0)
                {
                    strcpy(procList[i]->status, "Running");
                }
                else if(status == 1)
                {
                    strcpy(procList[i]->status, "Stopped");
                }
                else if(status == 2)
                {
                    strcpy(procList[i]->status, "Foreground");
                }
        }
    }
}

//adds an item to the process list, if it exists toggle between running and stopped
void addProc(int pid, char** inpt)
{
    //concatenate strings in inpt to get commandString
    int foreG = 1;
    char* commandString = (char*)malloc(1000*sizeof(char));
    strcpy(commandString, "\0");
    for(int i = 0; inpt[i] != NULL; i++)
    {
        if(strcmp(inpt[i], "&") == 0) foreG = 0;
        strcat(commandString, inpt[i]);
        strcat(commandString, " ");
    }

    //get number of pros
    int NumberOfProcess;
    if(procList == NULL)
        NumberOfProcess = 0;
    else
        for(NumberOfProcess = 0; procList[NumberOfProcess] != NULL; NumberOfProcess++);

        //define and initialize new proc
    struct proc* new = (struct proc*)malloc(sizeof(struct proc));
    new->number = NumberOfProcess + 1;
    strcpy(new->name, commandString);
    new->pid = pid;
    if(foreG) strcpy(new->status, "Foreground");
    else strcpy(new->status, "Running");

    //if procList is empty, allocate memory
    if(procList == NULL)
        procList = (struct proc**)calloc(MAX_JOBS, sizeof(struct proc*));

    //find position in procList in alphabetical order
    int pos = -1;
    int i;
    for(i = 0; procList[i] != NULL; i++)
    {
        if(strcmp(new->name, procList[i]->name) > 0)
        {
            continue;
        }
        else if(strcmp(new->name, procList[i]->name) == 0)
        {
            if(new->number<procList[i]->number)
            {
                pos = i;
                break;
            }
            else
                continue;
        }
        else if(strcmp(new->name, procList[i]->name) < 0)
        {
            pos = i;
            break;
        }
    }

    //insert at end
    if(pos == -1)
        pos = i;

    //shift elements and insert
    for(int j = NumberOfProcess; j > pos; j--)
        procList[j] = procList[j-1];
    procList[pos] = new;
    free(commandString);
}

//Clears procList and frees memory
void clearProc()
{
    if(procList == NULL)
        return;
    for(int i = 0; procList[i] != NULL; i++)
        free(procList[i]);
    free(procList); procList = NULL;
}

//removes pid from the procList
void removeProc(int pid)
{
    //empty list
    if(procList == NULL)
        return;
    int i;
    for(i = 0; procList[i] != NULL; i++)
    {
        if(procList[i]->pid == pid)
        {
            //shift all elements after i up
            int j = i+1;
            for(; procList[j] != NULL; j++) *procList[j-1] = *procList[j];
            free(procList[j-1]); procList[j-1] = NULL;
            break;
        }
    }

    //if empty now then clear
    if(procList[0] == NULL)
        clearProc();
}

//send signal to a child
void sendSignal(char ** inpt)
{
    if(getArgc(inpt) != 2)
    {
        fprintf(stderr, "Invalid number of arguments. \n");
        return;
    }
    int jobNo = atoi(inpt[1]);
    int sigNo = atoi(inpt[2]);
    int pidToSend = -1;

    //get pid corresponding to jobNo
    for(int i = 0; procList[i] != NULL; i++)
        if(procList[i]->number == jobNo)
            pidToSend = procList[i]->pid;
    if(pidToSend == -1)
    {
        fprintf(stderr, "Could not find given job.\n");
        return;
    }
    kill(pidToSend, sigNo); // The kill() system call can be used to send any signal to any process group or process.
}

//returns pid of foreground proc
int getForeProc()
{
    if(procList == NULL)
        return -1;
    int forePid = -1;
    for(int i = 0; procList[i] != NULL; i++)
        if(strcmp(procList[i]->status, "Foreground") == 0) forePid = procList[i]->pid;
    return forePid;
}

void makeBG(char** inpt)
{
    if(getArgc(inpt) != 1)
    {
        fprintf(stderr, "Invalid number of arguments.\n");
        return;
    }
    if(procList == NULL)
    {
        fprintf(stderr, "No background processes.\n");
        return;
    }
    long long num=atoi(inpt[1]);
    long long flag=0;
    for(long long i=1; i<=job_count; i++)
    {
        if(i==num)
        {
            flag=1;
            kill(job_arr[num].pid, SIGTTIN);
            kill(job_arr[num].pid,SIGCONT);
        }
    }
    if(flag==0)
        printf("Error: No such job found\n");
}

void makeFG(char** inpt)
{
    if(getArgc(inpt) != 1)
    {
        fprintf(stderr, "Invalid number of arguments.\n");
        return;
    }
    if(procList == NULL)
    {
        fprintf(stderr, "No background processes.\n");
        return;
    }

    long long num = atoi(inpt[1]);
    long long flag = 0;
    for (long long i = 1; i <= job_count; i++)
    {
        if (i == num)
        {
            signal(SIGTTIN, SIG_IGN);
            signal(SIGTTOU,SIG_IGN);
            tcsetpgrp(STDIN_FILENO,job_arr[num].pid);
            flag = 1;
            current_fore.pid=job_arr[num].pid;
            strcpy(current_fore.name,job_arr[num].name);
            kill(job_arr[num].pid, SIGCONT);
            del_process(job_arr[num].pid);  // DELETING THE PROCESS FROM THE BACKGROUND PROC ARRAY
            waitpid(-1, NULL, WUNTRACED);   // WAITING FOR THAT PROCESS TO FINISH EXECUTION
            tcsetpgrp(STDIN_FILENO,getpgrp());
            signal(SIGTTIN,SIG_DFL);
            signal(SIGTTOU,SIG_DFL);

        }
    }
    if (flag == 0)
        printf("Error: No such process found\n");
}