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
   mov r24,%1
   mov r25,%2
   ;
   ;  --- YOUR CODE GOES HERE ---
   ;       r24 = a, r25 = b
   ;

   push r22
   push r23
   push r24
   push r25
   call sumval
   mov r22,r24 ; not sure if we can store on r22/r23 here, but i hope so!
   mov r23,r25
   pop r25
   pop r24
   call diffval ; one qty to be added in r25:r24, other in r23:r22
   call wordadd
   pop r23 ; restore r23
   pop r22 ; restore r22
   rjmp end_of_assembly

wordadd:
   add r24,r22
   brcs wordoverflw ; lower-order bit needs a carry
   add r25,r23
   ret
wordoverflw:
   inc r23
   add r25,r23
   ret

sumval:
   add r24,r25
   clr r25 ; clear high order bit
   brcs overflw ; if carry bit is = 1, inc r25
   ret
overflw:
   inc r25
   ret

diffval:
   cp r25,r24
   brlo greater_lp
   sub r25,r24
   mov r24,r25
   clr r25
   ret
greater_lp:
   sub r24,r25
   clr r25
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
