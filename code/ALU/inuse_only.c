#include "main/main.c"

void spin_init(){

}
void spin_lock(){
	while(1){
		while(InUse!=0){
			//asm("pause");
			thrd_yield();
		}
		if(unlikely(InUse==0)){
            if(atomic_compare_exchange_weak(&InUse, &flag, 1)){
                return ;
            }
        }
		flag =0;
	}
}
void spin_unlock(){
	InUse=0;
}
void note_message(int thread_num, int nCS_size, int run_second){
    file_init();
	fprintf(fp,"=======================\n");
	fprintf(fp,"only inuse\n");
	fprintf(fp,"threads: %d\n",thread_num);
	fprintf(fp,"user_num: %d\n",user_num);
	fprintf(fp,"nCS_size: %d\n",nCS_size);
	fprintf(fp,"run_second: %d\n",run_second);
}
