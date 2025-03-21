
#include "queue.h"
#include "sched.h"
#include <pthread.h>

#include <stdlib.h>
#include <stdio.h>
static struct queue_t ready_queue;
static struct queue_t run_queue;
static pthread_mutex_t queue_lock;

#ifdef MLQ_SCHED
static struct queue_t mlq_ready_queue[MAX_PRIO];
#endif

int queue_empty(void) {
#ifdef MLQ_SCHED
	unsigned long prio;
	for (prio = 0; prio < MAX_PRIO; prio++)
		if(!empty(&mlq_ready_queue[prio])) 
			return -1;
#endif
	return (empty(&ready_queue) && empty(&run_queue));
}

int empty_queue(int a){
    unsigned long prio;
	for (prio = a; prio < MAX_PRIO; prio++){
		if(!empty(&mlq_ready_queue[prio])){
			return -1;
	        }
	}
	return 1;
}

void init_scheduler(void) {
#ifdef MLQ_SCHED
    int i ;

	for (i = 0; i < MAX_PRIO; i ++)
		mlq_ready_queue[i].size = 0;
#endif
	ready_queue.size = 0;
	run_queue.size = 0;
	pthread_mutex_init(&queue_lock, NULL);
}

#ifdef MLQ_SCHED

 int counter = 0;
 int loop = -1;
 int loop_state = 0;

struct pcb_t * get_mlq_proc(void) {
	 struct pcb_t * proc = NULL;

	 
        for (int counter0 = 0; counter0 < MAX_PRIO;counter0++){
            if (mlq_ready_queue[counter0].size > 0 && loop != counter0 && loop_state == 0 && mlq_ready_queue[counter0].slot >= 0){
                pthread_mutex_lock(&queue_lock );

	        proc = dequeue(&mlq_ready_queue[counter0]);

	        pthread_mutex_unlock(&queue_lock);
	        
	        mlq_ready_queue[counter0].slot -= 1;
	        
	        loop = counter0;
	        
	        loop_state = 1;
	 
            }else {
              loop = -1;
            }
        }
          
        for (int i = 0; i < MAX_PRIO; i++){
            mlq_ready_queue[i].slot = MAX_PRIO - i + 1;
        }
	 
	 loop_state = 0;
	 return proc;	
}

void put_mlq_proc(struct pcb_t * proc) {
	pthread_mutex_lock(&queue_lock);
	enqueue(&mlq_ready_queue[proc->prio], proc);
	pthread_mutex_unlock(&queue_lock);
}

void add_mlq_proc(struct pcb_t * proc) {
	pthread_mutex_lock(&queue_lock);
	enqueue(&mlq_ready_queue[proc->prio], proc);
	pthread_mutex_unlock(&queue_lock);	
}

struct pcb_t * get_proc(void) {
	return get_mlq_proc();
}

void put_proc(struct pcb_t * proc) {
	return put_mlq_proc(proc);
}

void add_proc(struct pcb_t * proc) {
	return add_mlq_proc(proc);
}
#else
struct pcb_t * get_proc(void) {
	struct pcb_t * proc = NULL;

	 pthread_mutex_lock(&queue_lock);

          proc = dequeue(&ready_queue);
  
	 pthread_mutex_unlock(&queue_lock);
	return proc;
}

void put_proc(struct pcb_t * proc) {
	pthread_mutex_lock(&queue_lock);
	enqueue(&run_queue, proc);
	pthread_mutex_unlock(&queue_lock);
}

void add_proc(struct pcb_t * proc) {
	pthread_mutex_lock(&queue_lock);
	enqueue(&ready_queue, proc);
	pthread_mutex_unlock(&queue_lock);	
}
#endif


