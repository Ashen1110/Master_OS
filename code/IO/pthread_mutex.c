#include "main/main.c"
pthread_mutex_t mutexlock;

void spin_init(){
	pthread_mutex_init(&mutexlock, PTHREAD_PROCESS_PRIVATE);
}
void spin_lock(){
	pthread_mutex_lock(&mutexlock);
}
void spin_unlock(){
	pthread_mutex_unlock(&mutexlock);
}
void note_message(int thread_num, int nCS_size, int run_second){
    file_init();
	fprintf(fp,"=======================\n");
	fprintf(fp,"pthread_mutex\n");
	fprintf(fp,"threads: %d\n",thread_num);
	fprintf(fp,"user_num: %d\n",user_num);
	fprintf(fp,"nCS_size: %d\n",nCS_size);
	fprintf(fp,"run_second: %d\n",run_second);
}
