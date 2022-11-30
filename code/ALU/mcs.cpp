#include "main/main.cpp"

struct mcs_spinlock{
    mcs_spinlock *next;
    atomic_bool locked;
};

#define mcs_null (mcs_spinlock*)NULL
mcs_spinlock* mcs_node;
thread_local mcs_spinlock* node;

void spin_init(){
    mcs_node = new mcs_spinlock;
    mcs_node = mcs_null;
}

void spin_lock(){
    node->next = mcs_null;
    mcs_spinlock* prev = atomic_compare_exchange_explicit(&mcs_node, node, memory_order_acq_rel);

    if(prev != mcs_null){
        node->locked = ATOMIC_VAR_INIT(false);
        atomic_store_explicit(&prev->next, node, memory_order_release);
        while(atomic_load_explicit(&node->locked, memory_order_acquire)) asm("pause");
    }
    
}

void spin_unlock(){
    mcs_spinlock* with_lock = atomic_load_explicit(&node->next, memory_order_acquire);
    if(with_lock == mcs_null){
        mcs_spinlock* prev = node;
        if(atomic_compare_exchange_weak(&mcs_node, &prev, mcs_null, memory_order_release, memory_order_relaxed)) return ;
        with_lock = atomic_load_explicit(&node->next, memory_order_acquire);
        while(mcs_null == with_lock) asm("pause");
    }
    atomic_store_explicit(&with_lock->locked, false, memory_order_release);
}