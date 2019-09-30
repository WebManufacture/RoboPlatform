stm8/
	#include "stm8s103F.inc"
	#include "stm8.inc"

	segment 'rom'

set_delay_ms.w
; IN X = Delay in ms 1..1000
   push A
   sllw X
   sllw X
   sllw X
   sllw X
   sllw X
   sllw X	
   ld A,#2
loop64.l
   srlw X
   inc A
   cpw X, #64
   jrule l64
   jra loop64
l64.l
   ld AWU_TBR,A
   ld A,XL
   ld AWU_APR,A
   pop A
   ret

delay.w
   mov AWU_CSR,#16
   HALT
   ret
	
wait.w
loop.l
	decw Y
  JRNE loop
	ret
	 
shim.w
blink.l
	 mov PA_ODR,#8 ; переключить светодиод 
	 ldw Y, $0
	 call wait   
	 
	 mov PA_ODR,#0 ; переключить светодиод 
	 ldw Y, $4
	 subw Y, $0
	 call wait
	 
	 decw X
	 jrne blink
	ret

	end
