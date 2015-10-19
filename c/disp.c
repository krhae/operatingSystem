 /* disp.c : dispatcher
  */
#include <stdlib.h>
#include <xeroskernel.h>


struct readyQueueItem *readyQueueHead;
struct readyQueueItem *readyQueueTail;

extern void init_disp() {
   readyQueueHead = NULL;
   readyQueueTail = NULL;
}


/*
 * An infinite loop which processes system calls, schedules the next
 * process, and calls the context switcher to switch into the next 
 * scheduled process.
 */
extern void dispatch(int *requests) {
   
   int *process = &next();
   
   while (1) {
       request = contextswitch(process);

       // Three possible requests: Create, Yield, Stop
       switch(request):

           case "CREATE":
                unsigned int const PROCESS_SIZE = 64;
                // Create a new process 
                int *newProcess = create(process, PROCESS_SIZE);
                //add it to the ready queue
                request(&newProcess);
                break;
           case "YIELD":
                // Place current process on end of ready queue
                request(&process);
                // Remove next process from ready queue & make new process
                process = &next();
                //TODO: Call the context switcher
                break; 
           case "STOP":
                // change process to stop
                kfree(process);
                process = &next();
                break;
   }
   
}

/*
 * Remove the next process from the end of the ready queue and 
 * return a pointer to its PCB.
 */
extern int *next( void ) {
   
   int *nextProcess = null;
   nextProcess = readyQueueHead->pcb;
   readyQueueHead = readyQueueHead->next;

   return nextProcess;
   
}

/* 
 * Take a pointer to a process's PCB and add it to the ready 
 * queue.
 */
extern void request( int * next ) {
   
   // Create a new queue item and initialise the values
   struct readyQueueItem *newItem = malloc(sizeof(struct readyQueueItem));
   (*newItem).pcb = &next;
   (*newItem).next = NULL;

   if(readyQueueHead == NULL) {
       readyQueueHead = newItem;
   }

   // Point the next item of the current tail to the new tail
   if(readyQueueTail!= NULL) {
       (*readyQueueTail).next = newItem;
   } else {
       readyQueueTail = newItem;       
   }

   // Change the tail to the new item
   readyQueueTail = newItem;
}