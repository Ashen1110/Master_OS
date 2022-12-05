#include "public.h"

double time_diff(struct timespec t1, struct timespec t2){
	struct timespec diff;
	if(t2.tv_nsec - t1.tv_nsec < 0){
		diff.tv_sec = t2.tv_sec - t1.tv_sec -1;
		diff.tv_nsec = t2.tv_nsec + 1000000000 - t1.tv_nsec;
	}
	else{
		diff.tv_nsec = t2.tv_nsec - t1.tv_nsec;
		diff.tv_sec = t2.tv_sec - t1.tv_sec;
	}
	double naro = diff.tv_nsec + diff.tv_sec*1000000000;
	return naro;
}

atomic_int counter = {0};

int error = 0;

void *dothread(void *arg){
	int num = atomic_fetch_add_explicit(&counter, 1, memory_order_release);
	num%=Num_core;
	cpu_set_t cpuset;
	CPU_ZERO(&cpuset);
	CPU_SET(num, &cpuset);
	sched_setaffinity(0, sizeof(cpuset), &cpuset);
	
    if(num!=sched_getcpu()){
		printf("false.\n");
	}//printf("%d ",num);
    
	double rnd;
	rnd=(double)(rand()%31)*0.01+0.85;
	rnd *= nCS_size;

	double dif_naro;
    struct timespec start, current, rs_start;

	clock_gettime(CLOCK_MONOTONIC, &start);
	clock_gettime(CLOCK_MONOTONIC, &current);
    // critical section
    while(running_flag=1){
		if(time_diff(start, current)> run_seconds*1000000000.0) break;
		//clock_gettime(CLOCK_MONOTONIC, &lock_begin);
		spin_lock() ; 
		//clock_gettime(CLOCK_MONOTONIC, &lock_end);
		//printf("%d\n", sched_getcpu());
		
		thread_cs_counter[num]++;
		
		error++;
		if(error!=1){
            printf("ERROR in CS of %d: %d\n",error,num);
        }
		for(int i=0;i<user_num;i++){
            (*(user+i))+=1;
			fprintf(test_file, "My user_num is %d\n", i);
		}
		rounds++;
		error--;
		
		//clock_gettime(CLOCK_MONOTONIC, &unlock_begin);
		spin_unlock() ; //US
		//clock_gettime(CLOCK_MONOTONIC, &unlock_end);

		//printf("spin lock: %.1f  spin unlock: %.1f\n", (time_diff(lock_begin, lock_end)), (time_diff(unlock_begin, unlock_end)));

		clock_gettime (CLOCK_MONOTONIC, &rs_start) ; //nCS
		
		do{
			clock_gettime(CLOCK_MONOTONIC, &current);
			dif_naro = time_diff(rs_start, current);
		}while(dif_naro < rnd);
		
	}
    pthread_exit(NULL);
    return (NULL);
}

int print_all();
void close_all(int sig){
	clock_gettime (CLOCK_MONOTONIC, &end_time) ;
	printf("\n");
	sleep(1);
	print_all();
	fclose(fp);
	fclose(test_file);
	printf("print finish.\n");
	exit(0);
	printf("impossible\n");
}

void write_result(long long int count, double time){
	double rate = (double)count/time;
	fprintf(fp, "count: %lld\ttime: %lf s\tcount/time: %lf \n", count, time, rate);
}
int print_all(){
	int data_flag= 1;
	for(int i=1; i<user_num; i++){
		if(*(user+i-1) != *(user+i)) {
			data_flag-=1;
			break;
		}
	}
	
	double exec_time = time_diff(begin_time, end_time)*0.000000001;
	printf("count: %lld\ttime: %lf s\t",rounds,exec_time);

	if(data_flag >=0){
		printf("amazing.");
	}else printf("data_flag: %d\n", data_flag);

	/*
	 for(int i=0; i<Num_core; i++){
	 	printf("thread %d enter CS: %d\n", i, thread_cs_counter[i]);
	 }
	*/
	
	write_result( rounds, exec_time);
	printf("\n");
	return data_flag;
}

void file_init(){
	if(fp!=NULL)
		fclose(fp);
	fp = fopen("result.txt", "a+");

	if(test_file != NULL) fclose(test_file);
	test_file = fopen("test_file.txt", "a+");
}

int main(int argc, char* argv[]){
    if(argc<2){
		printf("usage: <number of threads> <nCS_size>\n");
		return 0;
	}
	signal(SIGINT,close_all);
    //parameters
    int thread_num=atoi(argv[1]);
	nCS_size=atoi(argv[2]);
	
	double run_naro = run_seconds*1000000000.0;
    // output messages
    note_message(thread_num, nCS_size, run_seconds);
    file_init();

    user= malloc(user_num * sizeof(int));
	//new int[user_num]{0};
	for(int i=0;i<user_num;i++){
			user[i]=0;
	}
	for(int i=0; i<Num_core; i++){
		thread_cs_counter[i] = 0;
	}
	fp=NULL;
	test_file = NULL;
	file_init();

	int err=0;
	
	clock_gettime (CLOCK_MONOTONIC, &begin_time);
	spin_init();
	pthread_t* thread1 = (pthread_t*)malloc(sizeof(pthread_t) * thread_num);
	for(int i=0;i<thread_num;i++){
		err=pthread_create(&(thread1[i]), NULL, dothread, NULL);
		if(err!=0){
			printf("error in pthread create\n");
		}
	}
	printf("They are running.\n");
	for (int i=0; i<thread_num; i++) pthread_join(thread1[i], NULL);

	clock_gettime(CLOCK_MONOTONIC, &end_time);
	running_flag=0;
	
	sleep(2);
	if(print_all() >= 0) fprintf(fp,"correct.\n");
	else fprintf(fp,"data error.\n");
	
	fclose(fp);
	printf("One round finish.\n");
    return 0;
}
