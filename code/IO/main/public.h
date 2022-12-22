#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <threads.h>
#include <stdatomic.h>
#include <unistd.h>
#include <sys/types.h>
#include <sched.h>
#include <signal.h>
#include <time.h> 
#include <pthread.h>
#include <semaphore.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdint.h>
#include <sys/wait.h>
#define Num_core 64
//#define user_num 100
#define run_seconds 10
#define unlikely(x) __builtin_expect(!!(x), 0)

//parameters
int thread_num, nCS_size, user_num;

atomic_int InUse = ATOMIC_VAR_INIT(0); //anythread in CS or not

int* user;
int thread_cs_counter[Num_core];
long long int rounds = 0; // record the number of locks
//int run_core[Num_core];

struct timespec begin_time, end_time;
FILE * fp=NULL;
FILE * test_file=NULL;

int running_flag=1;


typedef struct mcs_spinlock{
    _Atomic (struct mcs_spinlock*) next;
    atomic_int locked;
}mcs_spinlock;

void spin_lock();
void mcs_spin_lock(struct mcs_spinlock *node);
void spin_unlock();
void mcs_spin_unlock(struct mcs_spinlock *node);
void spin_init();
void note_message(int thread_num, int nCS_size, int run_second);

thread_local int flag=0;
struct timespec lock_begin, lock_end;
struct timespec unlock_begin, unlock_end;

