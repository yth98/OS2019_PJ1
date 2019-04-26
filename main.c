// main.c : the Process Scheduler in user mode
// Operating System 2019, NTU CSIE
#include "main.h"

void err_sys(char *s) {
    perror(s);
    exit(1);
}

int cmp(const void *a, const void *b) {
	return ((proc_t *) a)->R - ((proc_t *) b)->R;
}

void unit_t(void) {
    volatile unsigned long i;
    for (i = 0; i < 1000000UL; i++);
};

int main(int argc, char const *argv[]) {
    // A child created via fork(2) inherits its parent's CPU affinity mask.
    cpu_set_t cmask;
    CPU_ZERO(&cmask);
    CPU_SET(0, &cmask); // only use cpu #0
    if (sched_setaffinity(getpid(), sizeof(cmask), &cmask) < 0)
        err_sys("sched_setaffinity");

    char S[8];
    int policy;
    scanf("%s", S);
    if (strcmp(S, "FIFO") == 0)
        policy = FIFO;
    else if (strcmp(S, "RR") == 0)
        policy = RR;
    else if (strcmp(S, "SJF") == 0)
        policy = SJF;
    else if (strcmp(S, "PSJF") == 0)
        policy = PSJF;
    else {
        fprintf(stderr, "Invalid policy.\n");
        exit(1);
    }

    int N;
    scanf("%d", &N);
    proc_t *proc;
    proc = (proc_t *) malloc(N * sizeof(proc_t));
    for (int i = 0; i < N; i++) {
        proc[i].pid = -1;
        scanf("%s %d %d", proc[i].N, &proc[i].R, &proc[i].T);
    }
    qsort(proc, N, sizeof(proc_t), cmp);

    // TODO: alter the loop to timeunit as index.
    for (long i=0;i<N;++i) {
        pid_t p = fork();
        // in child process, run the process for Y time units.
        if (p == 0) {
            // If goes here, I'm a child process!
            int Y = procs[i]->T;
            pid_t pid = getpid();
            FILE *proc;
            char instr[7];
            // inform the kernel module I'm start.
            proc = fopen("/proc/ospj1_proc", "a");
            sprintf(instr, "S%d", pid);
            fwrite(instr, sizeof(char), strlen(instr), proc);
            fclose(proc);
            // run for Y units.
            while (Y--) t_unit();
            // inform the kernel module I'm end.
            proc = fopen("/proc/ospj1_proc", "a");
            sprintf(instr, "F%d", pid);
            fwrite(instr, sizeof(char), strlen(instr), proc);
            fclose(proc);
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
