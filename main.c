// main.c : the Process Scheduler in user mode
// Operating System 2019, NTU CSIE
#include "main.h"

void err_sys(char *s) {
    perror(s);
    exit(1);
}

int cmp(const void *a, const void *b) {
    if (((proc_t *) a)->R - ((proc_t *) b)->R)
        return ((proc_t *) a)->R - ((proc_t *) b)->R;
    return ((proc_t *) a)->i - ((proc_t *) b)->i;
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

        cpu_set_t cmask;
        CPU_ZERO(&cmask);
        CPU_SET(1, &cmask); // only use cpu #1
        if (sched_setaffinity(mypid, sizeof(cmask), &cmask) < 0)
            err_sys("sched_setaffinity");

        set_low_priority(mypid); // wait until scheduler wakes me
        sched_yield();

        FILE *fp;
        char msg[256];
        struct timespec TS;

        // Record start time
        syscall(GETTIME, CLOCK_REALTIME, &TS);
        sprintf(msg, "[Project1] %d %lld.%.9ld", mypid, (long long)TS.tv_sec, TS.tv_nsec);

        for (int i = 1; i <= proc.T; i++) {
            unit_t();
            if (i % T_YIELD == 0)
                printf("%d$%d ",proc.i+1,i);
                sched_yield(); // switch back to scheduler
        }

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
    param.sched_priority = 99;
    if (sched_setscheduler(pid, SCHED_FIFO, &param) < 0) {
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

/*
proc_t *queue[64]; // waiting queue
short q_tail = 0;

void q_insert(proc_t* p, int policy) {
    queue[q_tail] = p;
    ++q_tail;
}
*/

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
        proc[i].i = i;
        scanf("%s %d %d", proc[i].N, &proc[i].R, &proc[i].T);
    }
    qsort(proc, N, sizeof(proc_t), cmp);

    set_high_priority(getpid());

    int time = 0; // current time
    int last = 0; // last context switch time
    int running = -1; // running process pid, -1 if no running process
    int created = 0; // number of created jobs
    int finished = 0; // number of finished jobs
    int next = -1; // next job to execute

    while (1) {
        if (running != -1 && proc[running].T == 0) {
            set_high_priority(proc[running].pid);
            waitpid(proc[running].pid, NULL, 0);
            proc[running].pid = -1;
            running = -1;
            finished += 1;
            if (finished == N)
                break;
        }
        // create jobs which R has arrived.
        while (created < N && proc[created].R <= time) {
            proc[created].pid = create_proc(proc[created]);
            //q_insert(&proc[created], policy);
            ++created;
        }
        next = pick_job(proc, N, policy, time, last, running);
        //printf("%d:%d ",time,next+1);
        if (next != -1 && next != running) {
            running = next;
            last = time;
        }
        if (running != -1) {
            // run the job for T_YIELD units and switch back
            // for synchronization
            set_high_priority(proc[running].pid);
            sched_yield();
            set_low_priority(proc[running].pid); // all job should keep inactive in usual
            if (proc[running].T > T_YIELD) {
                time += T_YIELD;
                proc[running].T -= T_YIELD;
            } else {
                time += proc[running].T;
                proc[running].T = 0;
            }
        }
        else { // tick per unit
            time += 1;
            unit_t();
        }
    }

    return 0;
}
