#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

enum Policy {FIFO, RR, SJF, PSJF};

typedef struct process {
    pid_t pid;
    int R, T;
    char N[33];
} t_proc;

void t_unit() { volatile unsigned long i; for(i=0;i<1000000UL;i++); }

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
            struct timespec then, now;
            //getnstimeofday(&then);
            int Y = procs[i]->T;
            while (Y--) t_unit();
            //getnstimeofday(&now);
            // https://stackoverflow.com/questions/8304259
            //printk("<6>[Project1] %d %lld.%.9ld %lld.%.9ld\n", getpid(), (long long)then.tv_sec, then.tv_nsec, (long long)now.tv_sec, now.tv_nsec);
            printf("<6>[Project1] %d %lld.%.9ld %lld.%.9ld\n", getpid(), (long long)then.tv_sec, then.tv_nsec, (long long)now.tv_sec, now.tv_nsec);
            return 0;
        }
        // in main process, record pids of children.
        else procs[i]->pid = p;
    }

    for (long i=0;i<N;++i)
        printf("%s %d\n", procs[i]->N, procs[i]->pid);

    return 0;
}