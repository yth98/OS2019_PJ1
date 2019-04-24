// proc_sort.c : sort processes with their ready time by HeapSort
// Operating System 2019, NTU CSIE
#include "proc_sort.h"

short proc_l (struct process* a, struct process* b) {return a->R<b->R || a->R==b->R && a->T<b->T;}
void proc_swap (struct process** a, struct process** b) {struct process* c = *a; *a = *b; *b = c;}

void proc_sort (struct process** _procs, int N) {
    // implement max-heap with an array
    //struct process* _heap[N];
    for (int i=1;i<=N;++i) {    // insert
        int j = i;
        do {
            if (proc_l(_procs[j/2-1], _procs[j-1])) proc_swap(&_procs[j/2-1], &_procs[j-1]);
            j /= 2;
        } while (j>0);
        for (int k=1;k<=N;++k) printf("%s\n", _procs[k-1]->N);
    }
    for (int i=N;i>=1;--i) {    // pop
        proc_swap(&_procs[0], &_procs[i-1]);
        int j = 1;
        do {
            short flag = 0;
            if (j*2<i && proc_l(_procs[j-1], _procs[j*2-1])) flag |= 1;
            if (j*2+1<i && proc_l(_procs[j-1], _procs[j*2])) flag |= 2;
            if (j*2+1<i && proc_l(_procs[j*2-1], _procs[j*2])) flag |= 4;
            switch (flag) {
                case 0: case 4:
                    j = i; break;
                case 1: case 3:
                    proc_swap(&_procs[j-1], &_procs[j*2-1]);
                    j = j*2; break;
                case 6: case 7:
                    proc_swap(&_procs[j-1], &_procs[j*2]);
                    j = j*2+1; break;
            }
        } while (j*2<i);
    }
}