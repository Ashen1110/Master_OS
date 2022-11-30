#include <iostream>
#include <thread>
#include <atomic>
#include <unistd.h>
#include <sys/types.h>
#include <sched.h>
#include <signal.h>
#include <stdlib.h>
#include <time.h> 
#include <pthread.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdint.h>
#include <sys/wait.h>
#define Num_core 64
#define user_num 100
#define run_seconds 60
#define unlikely(x) __builtin_expect(!!(x), 0)

using namespace std;

//parameters
int thread_num, nCS_size;

atomic_bool InUse = ATOMIC_VAR_INIT(false); //anythread in CS or not

int* user;
int thread_cs_counter[Num_core];
long long int rounds = 0; // record the number of locks
//int run_core[Num_core];

timespec begin_time, end_time;
FILE * fp=NULL;

bool running_flag=true;

void spin_lock();
void spin_unlock();
void spin_init();
void note_message(int thread_num, int nCS_size, int run_second);

thread_local bool flag=false;
timespec lock_begin, lock_end;
timespec unlock_begin, unlock_end;
