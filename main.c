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

void t_unit () { volatile unsigned long i; for(i=0;i<1000000UL;i++); }

int main() {
    // A child created via fork(2) inherits its parent's CPU affinity mask.
    cpu_set_t cmask;
    CPU_ZERO(&cmask);
    CPU_SET(0, &cmask); // only use cpu #0
    sched_setaffinity(getpid(), sizeof(cmask), &cmask);

    enum Policy S;
    char s[5];
    scanf("%s", s);
    if      (strcmp(s, "FIFO")==0) S = FIFO;
    else if (strcmp(s, "RR")==0)   S = RR;
    else if (strcmp(s, "SJF")==0)  S = SJF;
    else if (strcmp(s, "PSJF")==0) S = PSJF;

    int N;
    scanf("%d", &N);
    struct process* procs[N];
    for (long i=0;i<N;++i) {
        procs[i] = malloc(sizeof(t_proc));
        scanf("%s %d %d", procs[i]->N, &(procs[i]->R), &(procs[i]->T));
    }

    // TODO: alter the loop to timeunit as index.
    // sort procs by thier ready time R ?
    //     1. but the order of process name N will be missing.
    //        we should print them out in the end.
    //     2. but there's no STL sort() in C language.
    for (long i=0;i<N;++i) {
        pid_t p = fork();
        // in child process, run the process for Y time units.
        if (p == 0) {
            // If goes here, I'm a child process!
            int Y = procs[i]->T;
            pid_t pid = getpid();
            // TODO: inform the kernel module I'm start.
            while (Y--) t_unit();
            // TODO: inform the kernel module I'm end.
            return 0;
        }
        // in main process, record pids of children.
        procs[i]->pid = p;
        // in main process, specify each child's policy and param.
        // TODO: different actions with different S. ## The hardest part ##.
        struct sched_param spar;
        spar.sched_priority = 99;
        sched_setscheduler(p, SCHED_FIFO, &spar);
        // after we adjusted timeunit as index, uncomment the following line:
        //t_unit();
    }

    for (long i=0;i<N;++i)
        printf("%s %d\n", procs[i]->N, procs[i]->pid);

    return 0;
}