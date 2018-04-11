#include <stdio.h>
#include <avr/io.h>
#include "concurrency.h"

process_t * current_process = NULL; 

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

/* Called by the runtime system to select another process.
   "cursp" = the stack pointer for the currently running process
*/
process_t * dequeue (process_t * current) {
    process_t * head = current;
    current = current->next;
    head->next = NULL;
    return head;
}
void enqueue (process_t * current, process_t * old) {
        if(!current) { 
            current = old;
            return;
        } else {
            process_t * temp = current;
            while(temp->next != NULL) {
                temp = temp->next;
            }
            temp->next = old;
            return;
        }
}
__attribute__((used)) unsigned int process_select (unsigned int cursp) {
    // no current process
    if (!current_process) return 0;
        // not running yet
    if (cursp == 0)  return current_process->sp;
    // there is a process and go to next  
    else { 
        // remember current process and send to the end
        process_t * old = current_process; 
        old->sp = cursp; 
        if (current_process->block) {
            current_process = current_process->next;
            old->next = NULL;
            return current_process->sp;
        }

        // go to the next process
        current_process = current_process->next;
        
        // next is NULL
        old->next = NULL;
        
        // add to the end of the queue 
        enqueue(current_process, old);

        //return the next stack pointer
        return current_process->sp;
   }
}

/* Starts up the concurrent execution */
void process_start (void) {
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
    
    // if there are no current processes 
    if(!current_process) current_process = new_process;

    // else add to the end of the queue 
    else enqueue(current_process, new_process);
   
    return 0;
}

void lock_init (lock_t *l) {
    l->curr = NULL;
    l->locked = NULL;
}

void lock_acquire (lock_t *l) {
    if(!l->curr && current_process && !current_process->block) {
        l->curr = current_process;
        return;
    } else if(l->curr != current_process) {
        current_process->block = 1;
        enqueue(l->locked, current_process);
        yield();
    }
}

void lock_release (lock_t *l) {
    if(l->curr == current_process) {
        process_t * temp = l->locked;
        l->curr= NULL;
        l->locked = NULL;
        temp->block = 0;
        temp->next = current_process ;
        enqueue(current_process->next, temp);
    }
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
