#include <xeroskernel.h>
 
 /* Your code goes here - You will need to write some assembly code. You must
    use the gnu conventions for specifying the instructions. (i.e this is the
    compiler/assembler options or issue directives to permit usage of Intel's
    assembly language conventions.
 */

extern void contextinit() {
   // Set entry point in int table
   int const INT_NUM = 49;

   set_evec(INT_NUM, contextswitch);
}

// Need to save the current registers, stack pointer
extern void contextswitch( int *currentPCB ) {
    struct 

   static unsigned int saveESP = currentPCB->CPU->esp;
   static void *k_stack;
   static unsigned int interrupt, rc;

    // Push kernel state onto the stack
        // push flags
        // push registers
    __asm__volatile( " \
        jmp _disable \n\
        pushf \n\
        pusha \n\
        movl %%esp, k_stack \n\
        movl saveESP, %%esp \n\
        popa \n\
        iret \n\
        _InterruptEntryPoint: \n\
            pusha \n\
            movl $1, %%ecx \n\
            jmp _CommonJump \n\
        _ISREntryPoint: \n\
            pusha \n\
            movl $0, %%ecx \n\
        _CommonJump: \n\
            movl %%esp, saveESP \n\
            movl k_stack, %%esp \n\
            movl %%eax, rc \n\
            movl %%ecx, interrupt \n\
            popa \n\
            popf \n\
        "        
        : 
        :
        : "%eax"
    );

   // Save kernal stack pointer
   // Switch to process stack
   // save return value of syscall
   // pop process state from process stack
   // interupt return

   // ENTRY POINT:
   // Push process state onto process stack
   // Save process stack pointer
   // switch to kernel stack
   // save syscall args
   // pop kernel state from kernel stack
   // return from context switcher

}