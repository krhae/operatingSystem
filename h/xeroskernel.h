/* xeroskernel.h - disable, enable, halt, restore, isodd, min, max */

#ifndef XEROSKERNEL_H
#define XEROSKERNEL_H

/* Symbolic constants used throughout Xinu */

typedef	char    Bool;        /* Boolean type                  */
typedef unsigned int size_t; /* Something that can hold the value of
                              * theoretical maximum number of bytes 
                              * addressable in this architecture.
                              */
#define	FALSE   0       /* Boolean constants             */
#define	TRUE    1
#define	EMPTY   (-1)    /* an illegal gpq                */
#define	NULL    0       /* Null pointer for linked lists */
#define	NULLCH '\0'     /* The null character            */


/* Universal return constants */

#define	OK            1         /* system call ok               */
#define	SYSERR       -1         /* system call failed           */
#define	EOF          -2         /* End-of-file (usu. from read)	*/
#define	TIMEOUT      -3         /* time out  (usu. recvtim)     */
#define	INTRMSG      -4         /* keyboard "intr" key pressed	*/
                                /*  (usu. defined as ^B)        */
#define	BLOCKERR     -5         /* non-blocking op would block  */

/* Functions defined by startup code */


void           bzero(void *base, int cnt);
void           bcopy(const void *src, void *dest, unsigned int n);
void           disable(void);
unsigned short getCS(void);
unsigned char  inb(unsigned int);
void           init8259(void);
int            kprintf(char * fmt, ...);
void           lidt(void);
void           outb(unsigned int, unsigned char);
void           set_evec(unsigned int xnum, unsigned long handler);

// mem.c
extern void kmeminit( void );
extern void *kmalloc( int size );
extern void kfree( void *ptr );
struct memHeader {
  unsigned long size;
  struct memHeader *prev;
  struct memHeader *next;
  char *sanityCheck;
  unsigned char dataStart[0];
};

// create.c
extern int create( void (*func)(void), int stack );
extern void init_process_table( void );
extern int findEmptyPCBEntry( void ); 

static const int NUM_PROCESSES = 32;

struct pcb {
	int PID;
	int state;
	int parent_pid;
	int sp;
	struct context_frame *CPU;
	struct pcb *next;
};

struct context_frame {
	unsigned int edi;
	unsigned int esi;
	unsigned int ebp;
	unsigned int esp;
	unsigned int ebx;
	unsigned int edx;
	unsigned int ecx;
	unsigned int eax;
	unsigned int iret_eip;
	unsigned int iret_cs;
	unsigned int eflags;
};

static struct pcb pcbTable[NUM_PROCESSES];

//ctsw.c
extern void contextinit( void );
extern void contextswitch( int *currentProcess );

//disp.c
extern void init_disp( void );
extern void dispatch( int *requests );
extern int *next( void );
extern void request( int * next );

struct readyQueueItem {
   int *pcb;
   int *next;
};

//syscall.c
extern int syscall( int call, ... );
extern unsigned int syscreate( void (*func)(void), int stack );
extern void sysyield( void );
extern void sysstop( void );

//user.c
extern void root( void );
extern void producer( void );
extern void consumer( void );
/* Anything you add must be between the #define and this comment */
#endif
