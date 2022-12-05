#include "main/main.c"

atomic_llong global_ticket={0};
atomic_llong service_ticket={0};

void spin_init(){
    atomic_store_explicit(&global_ticket, 0, memory_order_release);
    atomic_store_explicit(&service_ticket, 0, memory_order_release);
}

void spin_lock(){
	long long int local_ticket;
    local_ticket = atomic_fetch_add_explicit(&global_ticket, 1, memory_order_relaxed);
    while(local_ticket != service_ticket) {
		//asm("pause");
		thrd_yield();
	} 
}

void spin_unlock(){
    atomic_fetch_add_explicit(&service_ticket, 1, memory_order_relaxed);
}

void note_message(int thread_num, int nCS_size, int run_second){
    file_init();
	fprintf(fp,"=======================\n");
	fprintf(fp,"ticket\n");
	fprintf(fp,"threads: %d\n",thread_num);
	fprintf(fp,"user_num: %d\n",user_num);
	fprintf(fp,"nCS_size: %d\n",nCS_size);
	fprintf(fp,"run_second: %d\n",run_second);
}
