#include "testasm.h"

/* the attribute "used" tells the compiler to not inline this function */
__attribute__((used))
unsigned int testasm(unsigned char a) {
  unsigned int out;

  asm volatile (R"(

   rjmp start_of_assembly

   .data
   ; any data values/static storage can go here

   .text

start_of_assembly:
   ; Your assembly language program goes here
   ;
   ; For this example, your program must not modify any registers other than 
   ; r24 and r25. In other words, save and restore all the registers that
   ; are modified by your code.

   ; Tell the compiler to move the arguments a, b into registers 
   ; r24, r25 respectively
   ;
   push r18
   mov r18, %1  
   ;
   ;  --- YOUR CODE GOES HERE ---
   ;       r24 = a, r25 = b
   ;

   ; counts the number of iterations the loop has gone through 
   push r19 
   ldi r19, 0
   
   ; holds the n-2 calculated value
   push r20
   ldi r20, 1 
   push r21 
   ldi r21, 0 

   ; holds the n-1 calculated value 
   push r22
   push r23 
   ldi r22, 0
   ldi r23, 0 

   ; holds the results 
   ldi r24, 0
   ldi r25, 0
   
   ; loop 
   fib:
     cp r19, r18
     brne fib_add
   
   ; pop everything off stack and end prgm
   finish:
     pop r23
     pop r22
     pop r21
     pop r20
     pop r19
     pop r18
     rjmp end_of_assembly ; end of prgm

   ; add n-1 and n-2 together
   fib_add:
     inc r19
     call wordadd ; add n-1 and n-2 to get result in r24 and r25
     mov r20, r22 ; move n-1 to n-2 
     mov r21, r23
     mov r22, r24 ; move result to n-1
     mov r23, r25 ; r22, r23 and r24, r25 are the same
     rjmp fib      ; recursive call back

   ; add two words together and manage overflow
   wordadd:
   add r24,r20 ; add lower bits
   clr r20 ; clear one register for carry bit addition
   adc r25,r20 ; carry bit + r25 + 0
   add r25,r21 ; dont care about high order overflow (not possible here)
   ret

   ;
   ;  --- YOUR CODE ENDS ---
   ;      The result must be in the register pair r25:r24
   ;      You can also rjmp to the label end_of_assembly if you need to.
   ;
end_of_assembly: 
   ; -- move r25:r24 to the 16-bit word in variable out
   movw %0,r24
)" : "=w" (out)  /* out is %0, and a 16-bit variable that is modified
		    by the assembly language */
   : "r" (a)  /* a is %1, b is %2, both are register-sized
			  variables */
   : "r25", "r24");   /* r24, r25 are modified by this assembly
			 language fragment */

  return out;
}
