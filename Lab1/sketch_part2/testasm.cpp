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
   
   push r22 ; save r22, r23 for later popping
   push r23
   push r24 ; save r24, r25 for subroutine diffval
   push r25
   call sumval ; perform first addition
   mov r22,r24 ; store return val from sumval
   mov r23,r25 ; store pt. 2
   pop r25 ; restore original a and b values
   pop r24
   call diffval ; one qty to be added in r25:r24, other in r23:r22
   call wordadd ; add r25:r24 to r23:r22
   pop r23 ; restore r23
   pop r22 ; restore r22
   rjmp end_of_assembly ; finished

wordadd:
   add r24,r22 ; add lower bits
   clr r22 ; clear one register for carry bit addition
   adc r25,r22 ; carry bit + r25 + 0
   add r25,r23 ; dont care about high order overflow (not possible here)
   ret

sumval:
   add r24,r25
   clr r25 ; clear high order bit
   adc r25,r25 ; add 1 or 0 to 0,0
   ret

diffval:
   cp r25,r24 ; compare b and a
   brlo greater_lp ; if b < a, goto greater_lp
   sub r25,r24 ; sub a from b
   mov r24,r25 ; move from higher-order register to lower-order register
   clr r25 ; clear higher-order bit
   ret
greater_lp: ; b < a
   sub r24,r25 ; sub b from a
   clr r25 ; clear higher-order bit
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
