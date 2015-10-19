/* create.c : create a process
 */
#include <xeroskernel.h>



// Creates a new process with code starting at *func amd intializes a stack space of stack
// Returns the process ID of the id if succesful, -1 if failed
extern int create( void (*func)(void), int stack ) {
	
	struct context_frame *contextFrame;	
	struct pcb *processBlock;		
	void *stackStart = kmalloc(stack);
	int pcbTableIndex = findEmptyPCBEntry();
	
	// Return 0 if mem alloc failed
	if(stackStart == 0) {
		return -1;
	}	

	// Return 0 if there is no room in the PCB Table
	if(pcbTableIndex == -1) {
		return -1;
	} else {
		// Else, add the PCB to the process table
		pcbTable[pcbTableIndex] = &processBlock;
	}

	
	// Initializing the context frame
	// Put the context at the top of the stack with a small buffer
	contextFrame = (((int)stackStart + stack) - sizeof(struct context_frame) - 16);	
	contextFrame->edi = 0;
	contextFrame->esi = 0;
	contextFrame->ebp = contextFrame - 16; // Is this right?
	// Set the sp to the end of the frame plus a small buffer
	contextFrame->esp = contextFrame - 16;
	contextFrame->ebx = 0;
	contextFrame->edx = 0;
	contextFrame->ecx = 0;
	contextFrame->eax = 0;
	// Set the next instruction to our process code
	contextFrame->iret_eip = func;
	// Get the code space
	contextFrame->iret_cs = (unsigned int)getCS();
	// Our flags are all zero as shown in class
	contextFrame->eflags = 0;
	
	// Configuring the process block
	//Set the stack pointer
	processBlock->sp = contextFrame - 16;
	// Copy in the context frame
	processBlock->CPU = contextFrame;
	//Set the state to ready
	processBlock->state = 1;
	// Is the PID preset?
	//processBlock->PID = pidCounter++;
	
	//ready(processBlock);
	
	return processBlock;	
	
}

extern void init_process_table() {
	for (int i=0; i < NUM_PROCESSES; i++) {
		pcbTable[i] = NULL;
	}
}

extern int findEmptyPCBEntry() {
	for(int i=0; i < NUM_PROCESSES; i++) {
		if(pcbTable[i] == NULL) {
			return i;
		}
	}
	return -1; // No PCB entries available
}
