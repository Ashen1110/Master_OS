#include "main/main.c"
sem_t semaphore;

void spin_init(){
	sem_init(&semaphore, 0, 1);
}
void spin_lock(){
	sem_wait(&semaphore);
}
void spin_unlock(){
	sem_post(&semaphore);
}
void note_message(int thread_num, int nCS_size, int run_second){
    file_init();
	fprintf(fp,"=======================\n");
	fprintf(fp,"binary_semaphore\n");
	fprintf(fp,"threads: %d\n",thread_num);
	fprintf(fp,"user_num: %d\n",user_num);
	fprintf(fp,"nCS_size: %d\n",nCS_size);
	fprintf(fp,"run_second: %d\n",run_second);
}
