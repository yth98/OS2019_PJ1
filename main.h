// main.h : the Process Scheduler in user mode
// Operating System 2019, NTU CSIE
#ifndef _USER_HEADER

#define _USER_HEADER
#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/syscall.h>
#include <time.h>
#include <sched.h>
#include <errno.h>

#define FIFO    0
#define RR      1
#define SJF     2
#define PSJF    3

#define T_YIELD 100
#define GETTIME __NR_clock_gettime // sys_clock_gettime

typedef struct process {
    pid_t pid;
    char N[64];
    int R, T, i;
} proc_t;

void err_sys(char *);

int cmp(const void *, const void *);

void unit_t(void);

pid_t create_proc(proc_t);

void set_high_priority(pid_t);

void set_low_priority(pid_t);

int pick_job(proc_t *, int, int, int, int, int);

int main(int, char **);

#endif
