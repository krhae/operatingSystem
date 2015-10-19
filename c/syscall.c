/* syscall.c : syscalls
 */

#include <xeroskernel.h>
#include <stdarg.h>
static void (*func)(void);
static int *stack;
static int *callId;
static int *result;

// This function is used to implement the different system calls
extern int syscall(int call, ...) {	
	*callId = call;
	switch( call ) {
		case '1' :
			asm( " \
				movl callId ,%%eax \n\
				int $44 \n\
				movl %%eax ,result \n\
				"		 
				: 
				:
				: "%eax"
				);
			break;
		case '2' :
			asm( " \
				movl callId ,%%eax \n\
				int $44 \n\
				movl %%eax ,result \n\
				"		 
				: 
				:
				: "%eax"
				);
			break;
		case '3' : 
			;
			va_list arglist;
			va_start(arglist, call);
			
			func = va_arg(arglist,void*);
			*stack = va_arg(arglist, int);
			
			asm( " \
				pushl func \n\
				pushl stack \n\
				movl callId ,%%eax \n\
				int $44 \n\
				movl %%eax ,result \n\
				"		 
				: 
				:
				: "%eax"
				);
			break;
	}	
	return *result;
}

// This function is used to create a process using the function specified in *func with stack memory
// Returns the PID
extern unsigned int syscreate( void (*func)(void), int stack ) {
	return syscall(1, func, stack);
}
// This function is used to yield the current process
extern void sysyield( void ) {
	syscall(2);
	return;
}
//This function is used to stop the current process
extern void sysstop( void ) {
	syscall(3);
	return;
}

