#include "main/main.c"
pthread_spinlock_t spin;

void spin_init(){
	pthread_spin_init(&spin, PTHREAD_PROCESS_PRIVATE);
}
void spin_lock(){
	pthread_spin_lock(&spin);
}
void spin_unlock(){
	pthread_spin_unlock(&spin);
}
void note_message(int thread_num, int nCS_size, int run_second){
    file_init();
	fprintf(fp,"=======================\n");
	fprintf(fp,"pthread_spin\n");
	fprintf(fp,"threads: %d\n",thread_num);
	fprintf(fp,"user_num: %d\n",user_num);
	fprintf(fp,"nCS_size: %d\n",nCS_size);
	fprintf(fp,"run_second: %d\n",run_second);
}
