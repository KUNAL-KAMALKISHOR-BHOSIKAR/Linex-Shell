//
// Created by kunal on 25/9/22.
//

#ifndef OSN_ASSIGNMENT2_JOBS_H
#define OSN_ASSIGNMENT2_JOBS_H

#include <sched.h>

typedef struct jobs{
    char name[10000];
    pid_t pid;
}job;

extern long long job_count;
extern struct jobs job_arr[100000];
extern struct jobs current_fore;

void AllJobs();

#endif //OSN_ASSIGNMENT2_JOBS_H
