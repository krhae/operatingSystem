/* user.c : User processes
 */

#include <xeroskernel.h>

extern void root( void ) {
	void (*prodPtr)(void) = &producer;
	void (*consPtr)(void) = &consumer;
	
	syscreate(prodPtr,4000);
	syscreate(consPtr,4000);
	
	for(;;) sysyield();

}

void producer( void ) {
	int i = 0;
	for(i=0;i++;i<12) {
		kprintf("Happy 100th");
		sysyield();
	}
	sysstop();
}

void consumer( void ) {
	int i = 0;
	for(i=0;i++;i<15) {
		kprintf("Birthday UBC \n");
		sysyield();
	}
	sysstop();
}