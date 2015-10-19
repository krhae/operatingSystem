/* mem.c : memory manager
 */

#include <xeroskernel.h>
#include <xeroslib.h>
#include <i386.h>

// Memory address representing the start of free memory
extern long freemem;
long freeMemListStart;
int memVerbose = 0;

// Initializes the memory manager
// Sets up linked list for keeping track of nodes
extern void
kmeminit (void)
{
    freeMemListStart = freemem;

    // set up first node
    struct memHeader *freeMemNode = freeMemListStart;
    freeMemNode->size = HOLESTART - freeMemListStart - sizeof(struct memHeader);
    freeMemNode->next = HOLEEND;

    // Set up node after hole
    freeMemNode = freeMemNode->next;
    freeMemNode->prev = freeMemListStart;
    freeMemNode->size = 4194304 - HOLEEND - sizeof(struct memHeader);
    if (memVerbose)
    {
        kprintf ("Memory initiation complete. Start address is: %04x \n",
            freemem);
    }
}


// Allocates a memory block of size bytes with a paragraph size of 16
// Returns 0 if insufficient memory
// Returns a pointer to the start of memory block if successful
extern void *kmalloc (int size)
{
    if (memVerbose)
    {
        kprintf ("Recieved memory request for size: %d \n", size);
    }
    long memAmount = size / 16 + ((size % 16) ? 1 : 0);
    memAmount = memAmount * 16 + sizeof (struct memHeader);
    if (memVerbose)
    {
        kprintf ("memAmount is: %d \n", memAmount);
    }

    struct memHeader *freeMemNode = freeMemListStart;
    if (memVerbose)
    {
        kprintf ("First node is at: %04x \n", freeMemListStart);
    }
    struct memHeader *usedMemNode;

    // Check all free memory nodes
    do
    {
        if (memVerbose)
        {
            kprintf ("Current node is: %04x \n", freeMemNode);
        }
        // Check if this node can hold this request for memory
        if (freeMemNode->size >= memAmount)
        {
            if (memVerbose)
            {
                kprintf ("Current Node can hold this request \n", 0);
            }
            long oldSize = freeMemNode->size;
            usedMemNode = freeMemNode;
            // Update size of used memory node
            usedMemNode->size = memAmount;

            // Move free memory node to new location
            if (memVerbose)
            {
                kprintf ("New node is at: %04x \n", freeMemNode + memAmount);
            }
            *(freeMemNode + memAmount) = *freeMemNode;
            freeMemNode = freeMemNode + memAmount;

            freeMemNode->size = oldSize - memAmount;
            if (memVerbose)
            {
                kprintf ("New node has memory size: %d \n", freeMemNode->size);
            }
            // Adjust previous nodes in list
            if (freeMemNode->prev == NULL)
            {
                //If first node change list start
                freeMemListStart = freeMemNode;
            }else
            {
                // Adjust previous node
                (freeMemNode->prev)->next = freeMemNode;
            }

            // Adjust following node
            if (freeMemNode->next != NULL)
            {
                (freeMemNode->next)->prev = freeMemNode;
            }

            usedMemNode->sanityCheck = usedMemNode->dataStart;
			
			
            return usedMemNode->dataStart;

        }

        // Check next node
        freeMemNode = freeMemNode->next;
    }
    while (freeMemNode != NULL);

    // Insufficient memory, return 0
    return 0;
}


// Returns memory at target address to free memory pool
// Adjacent memory will be coalesced
extern void kfree (void *ptr)
{
	struct memHeader *usedNode = ((unsigned int)ptr - sizeof(struct memHeader));
	if (memVerbose) {
		kprintf ("Used node at at: %04x \n", usedNode);
	}
	if( usedNode->sanityCheck != ptr) {
		
        kprintf ("Sanity check failed. Bad code! \n");

	}
    struct memHeader *memNode = freeMemListStart;
	
	// Check all nodes
	do {		
		  // Check if this memory can be coalesced
		if( (memNode < usedNode) && 
			(memNode + memNode->size) == usedNode ){		
			memNode->size = memNode->size + usedNode->size;	
			return;
		} else if( (memNode > usedNode) && 
			(usedNode + usedNode->size) == memNode ){
			// The used node absorbs the memnode if coalesced	
			usedNode->next = memNode->next;
			usedNode->prev = memNode->prev;
			
			// Update the next node
			(usedNode->next)->prev = usedNode;
			// update the prev node
			(usedNode->prev)->next = usedNode;
			usedNode->size = usedNode->size + memNode->size;
			
			if(usedNode->prev == NULL) {
				freeMemListStart = usedNode;		
			}				
			return;
		} else if( memNode > usedNode){		
			if(memNode->prev == NULL) {
				usedNode->prev = NULL;
				usedNode->next = memNode;
				memNode->prev = usedNode;
				freeMemListStart = usedNode;		
			} else {
				usedNode->prev = memNode->prev;
				usedNode->next = memNode;
				memNode->prev = usedNode;
			}						
			return;
		} else if(memNode < usedNode && memNode->next > usedNode ){
			// Update prev node of next node
			(memNode->next)->prev = usedNode;
			// Update node being freed
			usedNode->next = (memNode->next);
			usedNode->prev = memNode;
			// Update current node
			memNode->next = usedNode;	
			return;
		}
		// Check next node
		memNode = memNode->next;
	}while( memNode->next != NULL);	

	// If node goes at end of list
	memNode->next = usedNode;
	usedNode->next = NULL;
	usedNode->prev = memNode;
	return;		
}

// This function will print out a map of memory for all nodes
extern void kMemMapAll(char str[]) {
	struct memHeader *memNode = freeMemListStart;
	kprintf(str);
	int i = 0;
	
	do {
		kprintf ("Node[%d] is at: %04x \n",i, memNode);
		kprintf ("Node[%d] is %u bits \n",i, memNode->size);
		kprintf ("Node[%d]->prev is at: %04x \n",i, memNode->prev);
		kprintf ("Node[%d]->next is at: %04x \n",i, memNode->next);
		memNode = memNode->next;
		i++;
	}while( memNode != NULL);	
}
// This function will print out a map of memory for a specific node
extern void kMemMapOne(char str[], int index) {
	struct memHeader *memNode = freeMemListStart;
	kprintf(str);
	int i = 0;
	
	do {
		if(i == index) {
			kprintf ("Node[%d] is at: %04x \n",i, memNode);
			kprintf ("Node[%d] is %u bits \n",i, memNode->size);
			kprintf ("Node[%d]->prev is at: %04x \n",i, memNode->prev);
			kprintf ("Node[%d]->next is at: %04x \n",i, memNode->next);
		}
		memNode = memNode->next;
		i++;
	}while( memNode != NULL);	
}
