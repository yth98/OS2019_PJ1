#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sched.h>

enum Policy {FIFO, RR, SJF, PSJF};

typedef struct process {
    int R, T;
    char N[33];
} t_proc;

void t_unit() { volatile unsigned long i; for(i=0;i<1000000UL;i++); }

int main() {
    enum Policy S;
    char* s;
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

    /* test input/output */
    printf("Policy: %d\n", (int)S);
    for (long i=0;i<N;++i)
        printf("%s %d %d\n", procs[i]->N, procs[i]->R, procs[i]->T);
    /* delet this later. */

    return 0;
}