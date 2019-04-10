// main.h : the Process Scheduler in user mode
// Operating System 2019, NTU CSIE
#ifndef _USER_HEADER

#define _USER_HEADER
#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sched.h>

enum Policy {FIFO, RR, SJF, PSJF};

typedef struct process {
    pid_t pid;
    int R, T;
    char N[33];
} t_proc;

void t_unit ();

int main();

#endif