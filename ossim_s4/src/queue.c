#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

int empty(struct queue_t * q) {
        if (q == NULL) return 1;
	return (q->size == 0);
}

void enqueue(struct queue_t * q, struct pcb_t * proc) {
        /* TODO: put a new process to queue [q] */
        
        if (q == NULL || proc == NULL){
          return;
        }
        
        if (q->size >= MAX_QUEUE_SIZE){
          printf("Queue is full\n");
          return;
        }

        q->proc[q->size] = proc;
        q->size++;
}

struct pcb_t * dequeue(struct queue_t * q) {
        /* TODO: return a pcb whose prioprity is the highest
         * in the queue [q] and remember to remove it from q
         * */
          if (q->size <= 0){
         printf("Queue is empty\n");
            return NULL;
          }
          
              if (q->proc[0] == NULL) {
        printf("Error: Queue is NULL\n");
        return NULL;
    }
          //printf("ccccccccccccccccccccccccccccccc %d\n", q->size);
          struct pcb_t * highest_pri_pcb = q->proc[0];
              if (highest_pri_pcb == NULL) {
        printf("Error: First process is NULL\n");
        return NULL;
    }
    
            for(int i = 0; i < (q->size); i++){
              q->proc[i] = q->proc[i + 1];
            }
          q->proc[q->size -1] = NULL;
          q->size--;
          
          return highest_pri_pcb; 
}

