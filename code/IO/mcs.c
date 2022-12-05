#include "main/main_mcs.c"
#define mcs_null (struct mcs_spinlock*)NULL

_Atomic struct mcs_spinlock* mcs_node;

void spin_init(){
    mcs_node = malloc(sizeof(struct mcs_spinlock));
    mcs_node->next = mcs_null;
    mcs_node->locked = 0;  
}

void mcs_spin_lock(mcs_spinlock* node){
    
    atomic_init(&(node->next), mcs_null);
    mcs_spinlock* prev = __atomic_exchange_n(&(mcs_node->next), node, 0);

    if(prev != mcs_null){
        atomic_init(&(node->locked), 1);
        atomic_store_explicit(&(prev->next), node, memory_order_release);
        while(atomic_load_explicit(&(node->locked), memory_order_acquire)) {
            //asm("pause");
            thrd_yield();
        }

    }
    
}

void mcs_spin_unlock(mcs_spinlock* node){
    mcs_spinlock* with_lock = atomic_load_explicit(&(node->next), memory_order_acquire);
    if(with_lock == mcs_null){
        mcs_spinlock* prev = node;
        if(atomic_compare_exchange_weak(&(mcs_node->next), &prev, mcs_null)) return ;
        
        while((with_lock = atomic_load_explicit(&(node->next), memory_order_acquire)) == mcs_null) {
            //asm("pause");
            thrd_yield();
        }
    }
    atomic_store_explicit(&(with_lock->locked), 0, memory_order_release);
}

void note_message(int thread_num, int nCS_size, int run_second){
    file_init();
	fprintf(fp,"=======================\n");
	fprintf(fp,"mcs\n");
	fprintf(fp,"threads: %d\n",thread_num);
	fprintf(fp,"user_num: %d\n",user_num);
	fprintf(fp,"nCS_size: %d\n",nCS_size);
	fprintf(fp,"run_second: %d\n",run_second);
}
