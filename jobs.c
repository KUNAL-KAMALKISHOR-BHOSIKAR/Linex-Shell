//
// Created by kunal on 25/9/22.
//

#include "header.h"
#include "display.h"
#include "process.h"
#include "jobs.h"

//delete process from jobs
void del_process(int id)
{
    int flag = 0;
    if (id == -1)
        job_count = 0;
    else
        for (long long i = 1; i <= job_count; i++)
        {
            if (job_arr[i].pid == id)
            {
                flag = 1;
                for (long long j = i; j < job_count; j++)
                    job_arr[j] = job_arr[j + 1];
                job_count--;
            }
        }
    if (flag == 0 && id != -1)
        printf("Error: no such process found\n");
}

//background process end
void done()
{
    pid_t p;
    int status;
    p = waitpid(-1, &status, WNOHANG); // Wait for a child matching PID to die.
    for (long long z = 1; z <= job_count; z++)
    {
        if (p < 0)
        {
            perror("\nwaitpid failed\n");
        }
        const int exit = WEXITSTATUS(status);
        if (WIFEXITED(status) && p == job_arr[z].pid)
        {
            if (exit == 0)
                fprintf(stderr, "\nExited normally with exit status: %d\n", exit);
            else
                fprintf(stderr, "\nExited abnormally\n");

            fprintf(stderr, "%s with pid %d: exited\n", job_arr[z].name, p);
            PrintPrompt(homeDir,0);
            fflush(stdout);
            del_process(p);
        }
    }
}

void AllJobs()
{
    for(long long i=1; i<=job_count; i++)
    {
        long long pid=job_arr[i].pid;
        char name[10000],file[10000],status,s[100000];
        strcpy(name,job_arr[i].name);

        sprintf(file, "/proc/%lld/stat",pid);
        FILE *fd;
        fd=fopen(file,"r");
        if(fd<0)
            perror("Error: no such process exists\n");
        else
        {
            fscanf(fd, "%*d %*s %c %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d", &status);
            fclose(fd);
        }
        if(status=='T')
            strcpy(s,"Stopped");
        else
            strcpy(s,"Running");
        printf("[%lld] %s %s [%lld]\n",i, s, name,pid);
    }
}