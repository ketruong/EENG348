#include <stdio.h>
#include <avr/io.h>
#include "concurrency.h"
/*   current_process implements both the current_process global variable
 *   as well as the readyQ, where the readyQ is defined as
 *   current_process->next. tail enables fast dequeuing. run_flag is a
 *   global variable used so that process_select is able to distinguish
 *   between situations in which a process has just terminated or if the
 *   system has just been initialized.
 */
process_t * ready_queue = NULL;
process_t * tail = NULL;
process_t * current_process = NULL;
int run_flag = 0;

__attribute__((used)) unsigned char _orig_sp_hi, _orig_sp_lo;

__attribute__((used)) void process_begin ()
{
  asm volatile (
    "cli \n\t"
    "in r24,__SP_L__ \n\t"
    "sts _orig_sp_lo, r24\n\t"
    "in r25,__SP_H__ \n\t"
    "sts _orig_sp_hi, r25\n\t"
    "ldi r24, 0\n\t"
    "ldi r25, 0\n\t"
    "rjmp .dead_proc_entry\n\t"
    );
}

__attribute__((used)) void process_terminated ()
{
  asm volatile (
    "cli\n\t"
    "lds r25, _orig_sp_hi\n\t"
    "out __SP_H__, r25\n\t"
    "lds r24, _orig_sp_lo\n\t"
    "out __SP_L__, r24\n\t"
    "ldi r24, lo8(0)\n\t"
    "ldi r25, hi8(0)\n\t"
    "rjmp .dead_proc_entry"
    );
}

void process_timer_interrupt ();

__attribute__((used)) void yield ()
{
  if (!current_process) return;
  asm volatile ("cli\n\t");
  asm volatile ("rjmp process_timer_interrupt\n\t");
}

__attribute__((used)) void process_timer_interrupt()
{
  asm volatile (
    "push r31\n\t"
    "push r30\n\t"
    "push r29\n\t"
    "push r28\n\t"
    "push r27\n\t"
    "push r26\n\t"
    "push r25\n\t"
    "push r24\n\t"
    "push r23\n\t"
    "push r22\n\t"
    "push r21\n\t"
    "push r20\n\t"
    "push r19\n\t"
    "push r18\n\t"
    "push r17\n\t"
    "push r16\n\t"
    "push r15\n\t"
    "push r14\n\t"
    "push r13\n\t"
    "push r12\n\t"
    "push r11\n\t"
    "push r10\n\t"
    "push r9\n\t"
    "push r8\n\t"
    "push r7\n\t"
    "push r6\n\t"
    "push r5\n\t"
    "push r4\n\t"
    "push r3\n\t"
    "push r2\n\t"
    "push r1\n\t"
    "push r0\n\t"
    "in r24, __SREG__\n\t"
    "push r24\n\t"
    "in r24, __SP_L__\n\t"
    "in r25, __SP_H__\n\t"
    ".dead_proc_entry:\n\t"
    "rcall process_select\n\t"
    "eor r18,r18\n\t"
    "or r18,r24\n\t"
    "or r18,r25\n\t"
    "brne .label_resume\n\t"
    "lds r25, _orig_sp_hi\n\t"
    "out __SP_H__, r25\n\t"
    "lds r24, _orig_sp_lo\n\t"
    "out __SP_L__, r24\n\t"
    "ret\n\t"
    ".label_resume:\n\t"
    "out __SP_L__, r24\n\t"
    "out __SP_H__, r25\n\t"
    "pop r0\n\t"
    "out  __SREG__, r0\n\t"
    "pop r0\n\t"
    "pop r1\n\t"
    "pop r2\n\t"
    "pop r3\n\t"
    "pop r4\n\t"
    "pop r5\n\t"
    "pop r6\n\t"
    "pop r7\n\t"
    "pop r8\n\t"
    "pop r9\n\t"
    "pop r10\n\t"
    "pop r11\n\t"
    "pop r12\n\t"
    "pop r13\n\t"
    "pop r14\n\t"
    "pop r15\n\t"
    "pop r16\n\t"
    "pop r17\n\t"
    "pop r18\n\t"
    "pop r19\n\t"
    "pop r20\n\t"
    "pop r21\n\t"
    "pop r22\n\t"
    "pop r23\n\t"
    "pop r24\n\t"
    "pop r25\n\t"
    "pop r26\n\t"
    "pop r27\n\t"
    "pop r28\n\t"
    "pop r29\n\t"
    "pop r30\n\t"
    "pop r31\n\t"
    "reti\n\t");
}

/*
 * Stack: save 32 regs, +2 for entry point +2 for ret address
 */
#define EXTRA_SPACE 37
#define EXTRA_PAD 4


void enqueue_ready (process_t * old) {
    if (tail->prio >= old->prio) {
        tail->next = old;
        tail = tail->next;
    }  else if(old->prio > ready_queue->prio) {
        old->next = ready_queue;
        ready_queue = old;
    } else {
       process_t * temp = ready_queue;
       while (temp->next && temp->next->prio > old->prio) temp=temp->next;  
       old->next = temp->next;
       temp->next = old;
    }
}

void enqueue_curr (process_t * old) {
    if (tail->prio >= old->prio) {
        tail->next = old;
        tail = tail->next;
    } else if(old->prio > old->prio) {
        old->next = current_process;
        current_process = old;
    } else {
       process_t * temp = current_process;
       while (temp->next && temp->next->prio > old->prio) temp=temp->next;  
       old->next = temp->next;
       temp->next = old;
    }
}


// add some process old to lock queue
void enqueue_locked (process_t * locked, process_t * old) {
    process_t * temp = locked;
    while (temp->next) temp = temp->next;
    temp->next = current_process;
}

/* Called by the runtime system to select another process.
   "cursp" = the stack pointer for the currently running process
*/
__attribute__((used)) unsigned int process_select (unsigned int cursp) {
    
    // run_flag not yet raised, so need to initialize stack pointer
    if (!cursp && !run_flag) {
       run_flag = 1;
       return current_process->sp;

    // a process has terminated, so need to free it and pop a new process from readyQ
    } else if (!cursp && run_flag && current_process) {
       current_process = ready_queue;
       process_t * old = current_process; 
       current_process = current_process->next;
       ready_queue = current_process;
       current_process = old;
       old->next = NULL;
       free(old->sp);
       free(old);
       return ready_queue->sp;
       
    // else there is a current process, and we need to get the next process from the queue
    } else { 
        current_process = ready_queue;
        
        // remember current process and send to the end
        process_t * old = current_process; 
        old->sp = cursp; 
        
        // if there exists only one process, return its sp and resume progress on it
        if(!current_process->next) return old->sp;

        // go to the next process
        current_process = current_process->next;
        
        // next on the old process points to NULL
        // necessary because we're about to enqueue it
        old->next = NULL;
        
        // add to the end of the queue 
        if(!old->block) enqueue_curr(old);
        
        ready_queue = current_process;
        //return the next stack pointer
        return current_process->sp;
   }
}

/* Starts up the concurrent execution */
void process_start (void) {
    current_process = ready_queue;
    if (!current_process) return;
    process_begin();
}

/* Create a new process */
int process_create (void (*f)(void), int n) {
    // allocate stack and get stack pointer
    unsigned int new_sp = process_init(f,n);
    
    // if stack could not be allocated, error 
    if(!new_sp) return -1;

    // malloc for new process 
    process_t* new_process = (process_t *) calloc(1, sizeof(process_t));

    // if calloc fails, error
    if(!new_process) return -1;
    
    // set fields
    new_process->sp = new_sp;
    new_process->next = NULL;
    new_process->block = 0;
    new_process->prio = 128;
    
    // if there are no current processes 
    if(!ready_queue) {
      ready_queue = new_process;
      tail = ready_queue;
    }

    // else add to the end of the queue 
    else enqueue_ready(new_process);
   
    return 0;
}

/* Create a new process with a priority */
int process_create_prio (void (*f)(void), int n, unsigned char prio) {
// allocate stack and get stack pointer
    unsigned int new_sp = process_init(f,n);
    
    // if stack could not be allocated, error 
    if(!new_sp) return -1;

    // malloc for new process 
    process_t* new_process = (process_t *) calloc(1, sizeof(process_t));

    // if calloc fails, error
    if(!new_process) return -1;
    
    // set fields
    new_process->sp = new_sp;
    new_process->next = NULL;
    new_process->block = 0;
    new_process->prio = prio;
    
    // if there are no current processes 
    if(!ready_queue) {
      ready_queue = new_process;
      tail = ready_queue;
    }

    // else add to the end of the queue 
    else enqueue_ready(new_process);
   
    return 0;
}

int process_create_rtjob (void (*f)(void), int n, unsigned int wcet, unsigned int deadline) {
}


// lock_init sets the current lock queue to NULL
void lock_init (lock_t *l) {
    l->curr = NULL;
    l->locked = NULL;
}

// current_process attempts to acquire the lock
void lock_acquire (lock_t *l) {
    asm volatile ("cli\n\t"); // clear interrupts
   
    // process gets the lock if no other process has the lock
    if(!l->curr) l->curr = current_process;
    
    // if another process already has the lock
    else if(l->curr != current_process) {
      
        // add current_process to lock queue
        // if there's nothing in the queue, head of the queue = current_process
        if(!l->locked) l->locked = current_process;
        // else add it to the queue
        else enqueue_locked(l->locked, current_process);
 
        // this process is blocked, so update its blocked var
        current_process->block = 1;
        
        // go to the next process 
        yield();
    }
    asm volatile ("sei\n\t");
}

// current process gives up the lock
void lock_release (lock_t *l) {
    asm volatile ("cli\n\t");
    current_process = ready_queue;
    // if there's nothing in the queue, queue head is set to NULL
    if(!l->locked) {
      l->curr = NULL;
      return;
    }
    
    // there's something in the waiting queue, and next queued process gets the lock 
    l->curr = l->locked;
    
    // get first item off the locked (waiting) queue
    process_t * temp  = l->locked;
    process_t * old = current_process;

    // increment the locked (waiting) queue
    l->locked = l->locked->next; 
    
    // prep dequeued process for placement at the end of the ready queue
    temp->next = NULL;
    
    // dequeued process is no longer blocked 
    temp->block = 0;

    // push dequeued process onto the ready queue
    if(!ready_queue) ready_queue = temp; 
    else enqueue_ready(temp);

    ready_queue = current_process;
    asm volatile ("sei\n\t");
}


unsigned int process_init (void (*f) (void), int n)
{
  unsigned long stk;
  int i;
  unsigned char *stkspace;

  /* Create a new process */
  n += EXTRA_SPACE + EXTRA_PAD;
  stkspace = (unsigned char *) malloc (n);

  if (stkspace == NULL) {
    /* failed! */
    return 0;
  }

  /* Create the "standard" stack, including entry point */
  for (i=0; i < n; i++) {
      stkspace[i] = 0;
  }

  n -= EXTRA_PAD;

  stkspace[n-1] = ( (unsigned int) process_terminated ) & 0xff;
  stkspace[n-2] = ( (unsigned int) process_terminated ) >> 8;
  stkspace[n-3] = ( (unsigned int) f ) & 0xff;
  stkspace[n-4] = ( (unsigned int) f ) >> 8;

  /* SREG */
  stkspace[n-EXTRA_SPACE] = SREG;

  stk = (unsigned int)stkspace + n - EXTRA_SPACE - 1;

  return stk;
}
