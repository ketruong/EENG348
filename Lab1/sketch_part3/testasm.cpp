#include "testasm.h"

/* the attribute "used" tells the compiler to not inline this function */
__attribute__((used))
unsigned int testasm(unsigned char a, unsigned char b) {
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
   mov r18, %1 ; 
   ;
   ;  --- YOUR CODE GOES HERE ---
   ;       r24 = a, r25 = b
   ;

   ; counter  
   push r19 
   ldi r19, 0
   
   ; n-2 
   push r20
   ldi r20, 1 
   push r21 
   ldi r21, 0 

   ; n-1
   push r22
   push r23 
   ldi r22, 0
   ldi r23, 0 

   ; result
   ldi r24, 0
   ldi r25, 0

   ; check for 0
   cpi r18, 0
   brne one
   rjmp finish
   
   one:
   ; check for 1 
   cpi r18, 1
   brne fib
   ldi r24, 1
   rjmp finish
   
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
     rjmp end_of_assembly

   fib_add:
     inc r19
     call wordadd
     mov r20, r22
     mov r21, r23
     mov r22, r24
     mov r23, r25
     rjmp fib

   ; add two words together and manage overflow
   wordadd:
     add r24,r20
     brcs wordoverflw ; lower-order bit needs a carry
     add r25,r21
     ret
   
   wordoverflw:
     inc r21
     add r25,r21
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
   : "r" (a), "r" (b)  /* a is %1, b is %2, both are register-sized
			  variables */
   : "r25", "r24");   /* r24, r25 are modified by this assembly
			 language fragment */

  return out;
}
