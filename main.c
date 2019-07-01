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
    return;
};

pid_t create_proc(proc_t proc) {
    pid_t pid = fork();
    if (pid < 0)
        err_sys("fork");
    else if (pid == 0) {
        pid_t mypid = getpid();
        printf("%s %d\n", proc.N, mypid);

        FILE *fp;
        char msg[256];
        struct timespec TS;

        // Record start time
        syscall(GETTIME, CLOCK_REALTIME, &TS);
        sprintf(msg, "[Project1] %d %lld.%.9ld", mypid, (long long)TS.tv_sec, TS.tv_nsec);

        for (int i = 0; i < proc.T; i++)
            unit_t();

        // Record end time
        syscall(GETTIME, CLOCK_REALTIME, &TS);
        sprintf(msg, "%s %lld.%.9ld\n", msg, (long long)TS.tv_sec, TS.tv_nsec);

        fp = fopen("/dev/kmsg", "a");
        fprintf(fp, "%s", msg);
        fclose(fp);

        exit(0);
    }
    else
        return pid;
}

void set_high_priority(pid_t pid) {
    struct sched_param param;
    param.sched_priority = 0;
    if (sched_setscheduler(pid, SCHED_OTHER, &param) < 0) {
        fprintf(stderr, "%d\n", pid);
        err_sys("sched_setscheduler (high)");
    }
    return;
}

void set_low_priority(pid_t pid) {
    struct sched_param param;
    param.sched_priority = 0;
    if (sched_setscheduler(pid, SCHED_IDLE, &param) < 0) {
        fprintf(stderr, "%d\n", pid);
        err_sys("sched_setscheduler (low)");
    }
    return;
}

int pick_job(proc_t *proc, int N, int policy, int time, int last, int running) {
    int pick = -1;
    if (policy == FIFO) {
        if (running != -1)
            return running;
        for (int i = 0; i < N; i++)
            if (proc[i].pid != -1)
                return i;
    }
    if (policy == RR) {
        if (running != -1) {
            if ((time - last) % 500 == 0) {
                pick = (running + 1) % N;
                while (proc[pick].pid == -1)
                    pick = (pick + 1) % N;
            }
            else
                return running;
        }
        else
            for (int i = 0; i < N; i++)
                if (proc[i].pid != -1)
                    return i;
    }
    if (policy == SJF) {
        if (running != -1)
            return running;
        for (int i = 0; i < N; i++)
            if (proc[i].pid != -1)
                if (pick == -1 || proc[i].T < proc[pick].T)
                    pick = i;
    }
    if (policy == PSJF) {
        for (int i = 0; i < N; i++)
            if (proc[i].pid != -1)
                if (pick == -1 || proc[i].T < proc[pick].T)
                    pick = i;
    }
    return pick;
}

int main(int argc, char *argv[]) {
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
    else
        err_sys("Invalid policy.");

    int N;
    scanf("%d", &N);
    proc_t *proc;
    proc = (proc_t *) malloc(N * sizeof(proc_t));
    for (int i = 0; i < N; i++) {
        proc[i].pid = -1;
        scanf("%s %d %d", proc[i].N, &proc[i].R, &proc[i].T);
    }
    qsort(proc, N, sizeof(proc_t), cmp); // TODO: stable sorting

    set_high_priority(getpid());

    int time = 0; // current time
    int last = 0; // last context switch time
    int running = -1; // running process pid, -1 if no running process
    int finished = 0; // number of finished jobs
    int next = -1; // next job to execute

    while (1) {
        if (running != -1 && proc[running].T == 0) {
            waitpid(proc[running].pid, NULL, 0);
            proc[running].pid = -1;
            running = -1;
            finished += 1;
            if (finished == N)
                break;
        }
        for (int i = 0; i < N; i++)
            if (proc[i].R == time) {
                proc[i].pid = create_proc(proc[i]);
                set_low_priority(proc[i].pid);
            }
        next = pick_job(proc, N, policy, time, last, running);
        if (next != -1 && next != running) {
            set_low_priority(proc[running].pid);
            set_high_priority(proc[next].pid);
            running = next;
            last = time;
        }
        if (running != -1)
            proc[running].T -= 1;
        time += 1;
        unit_t();
    }

    return 0;
}
