/*************************************************************************
 *
 *  Copyright (c) 2013--2018 Rajit Manohar
 *  All Rights Reserved
 *
 **************************************************************************
 */
#ifndef __CONCURRENCY_H__
#define __CONCURRENCY_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef struct process_state {
    unsigned int sp; /* stack pointer */
    struct process_state *next; /* link to next process */
    int block;
    unsigned int prio;
    unsigned int deadline;
    unsigned int wcet;
    void (*f)(void);
    unsigned int started;
    unsigned int elapsed;
} process_t;

typedef struct lock_state {
    struct process_state * curr; /* link to next process */
    struct process_state * locked;
} lock_t;

/* opaque definition of process type; you must provide this
   implementation.
*/


/*------------------------------------------------------------------------

   THE FOLLOWING FUNCTIONS MUST BE PROVIDED.

------------------------------------------------------------------------*/


/* processes */ 

extern process_t * current_process; 
extern process_t * tail; 

__attribute__((used)) unsigned int process_select (unsigned int cursp);
/* Called by the runtime system to select another process.
   "cursp" = the stack pointer for the currently running process
*/

void process_start (void);
/* Starts up the concurrent execution */

int process_create (void (*f)(void), int n);
/* Create a new process */

int process_create_prio (void (*f)(void), int n, unsigned char prio); 
/* Create a new process with priority  */

int process_create_rtjob (void (*f)(void), int n ,unsigned int wcet, unsigned int deadline);
/* Create a new process with priority based on */

/* lock */
void lock_init (lock_t *l);
void lock_acquire (lock_t *l);
void lock_release (lock_t *l);


/*-- functions provided in the .c file --*/

unsigned int process_init (void (*f) (void), int n);
void process_begin ();
void process_timer_interrupt();
void yield ();

#ifdef __cplusplus
}
#endif

#endif /* __CONCURRENCY_H__ */



