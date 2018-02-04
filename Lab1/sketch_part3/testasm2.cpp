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

init:
   push r23
   push r26
   clr r26
   mov r23,r24
   ldi r25,1
   clr r24

   cpi r23,0
   breq post_fin

loop:
   cpi r23,1
   breq fin1

   add r24,r25
   add r26,r25
   dec r23

   cpi r23,1
   breq fin2

   add r25,r24
   add r26,r24
   dec r23
   rjmp loop

fin1:
   add r26, r25
   rjmp post_fin

fin2:
   add r26, r24
   rjmp post_fin

post_fin:
   mov r24,r26
   clr r25
   pop r26
   pop r23

   
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
