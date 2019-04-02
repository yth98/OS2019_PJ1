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

int main() {
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

    for (long i=0;i<N;++i) {
        pid_t p = fork();
        // in child process, run the process for Y time units.
        if (p == 0) {
            int Y = procs[i]->T;
            pid_t pid = getpid();
            /* do something what to tell kernel module opening a child process */
            return 0;
        }
        // in main process, record pids of children.
        else procs[i]->pid = p;
    }

    for (long i=0;i<N;++i)
        printf("%s %d\n", procs[i]->N, procs[i]->pid);

    return 0;
}