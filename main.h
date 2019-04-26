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
#include <errno.h>

#define FIFO    0
#define RR      1
#define SJF     2
#define PSJF    3

typedef struct process {
    pid_t pid;
    char N[64];
    int R, T;
} proc_t;

void err_sys(char *);

int cmp(const void *, const void *);

void unit_t(void);

pid_t create_proc(proc_t);

void set_high_priority(pid_t);

void set_low_priority(pid_t);

int main(int, char **);

#endif
