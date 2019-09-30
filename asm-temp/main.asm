stm8/

	#include "mapping.inc"
	#include "stm8s103F.inc"
  #include "stm8.inc"
  
	segment 'rom'
  
wait.w
  ldw x, #20
loop1.l
  ldw Y, #16535
loop2.l
	decw Y
  JRNE loop2
	decw X
  JRNE loop1
  ret

  
main.l
	; initialize SP
  BSET PA_DDR,#3
  
  BSET PD_CR1,#4 
  BSET PD_CR2,#4
  BSET PD_DDR, #4
  
  MOV PC_DDR,#{bit3 + bit4 + bit5 + bit6 + bit7}
  MOV PC_CR1,#{bit3 + bit4 + bit5 + bit6 + bit7}
  MOV PC_CR2,#{bit3 + bit4 + bit5 + bit6 + bit7}
  
  BSET PA_ODR, #3
  BSET PD_ODR, #4
  BSET PC_ODR, #3
  BRES PD_ODR, #4
  MOV PC_ODR,#0
  
infinite_loop.l
	 mov PC_ODR,#240 ; переключить светодиод 
	 call wait   
	 
	 mov PC_ODR,#~bit4 ; переключить светодиод 
	 call wait   
   
	 mov PC_ODR,#~bit5 ; переключить светодиод 
	 call wait   
   
	 mov PC_ODR,#~bit6 ; переключить светодиод 
	 call wait   
   
	 mov PC_ODR,#~bit7 ; переключить светодиод 
	 call wait   
   
	 mov PC_ODR,#0 ; переключить светодиод 
	 mov PD_ODR,#~bit2 ; переключить светодиод 
	 call wait   
	 
   mov PD_ODR,#~bit3 ; переключить светодиод 
	 call wait   
   
   mov PD_ODR,#~bit4 ; переключить светодиод 
	 call wait  
	 
   mov PD_ODR,#255; переключить светодиод 
	 
	jra infinite_loop

	interrupt NonHandledInterrupt
NonHandledInterrupt.l
	iret

	segment 'vectit'
	dc.l {$82000000+main}									; reset
	dc.l {$82000000+NonHandledInterrupt}	; trap
	dc.l {$82000000+NonHandledInterrupt}	; irq0
	dc.l {$82000000+NonHandledInterrupt}	; irq1
	dc.l {$82000000+NonHandledInterrupt}	; irq2
	dc.l {$82000000+NonHandledInterrupt}	; irq3
	dc.l {$82000000+NonHandledInterrupt}	; irq4
	dc.l {$82000000+NonHandledInterrupt}	; irq5
	dc.l {$82000000+NonHandledInterrupt}	; irq6
	dc.l {$82000000+NonHandledInterrupt}	; irq7
	dc.l {$82000000+NonHandledInterrupt}	; irq8
	dc.l {$82000000+NonHandledInterrupt}	; irq9
	dc.l {$82000000+NonHandledInterrupt}	; irq10
	dc.l {$82000000+NonHandledInterrupt}	; irq11
	dc.l {$82000000+NonHandledInterrupt}	; irq12
	dc.l {$82000000+NonHandledInterrupt}	; irq13
	dc.l {$82000000+NonHandledInterrupt}	; irq14
	dc.l {$82000000+NonHandledInterrupt}	; irq15
	dc.l {$82000000+NonHandledInterrupt}	; irq16
	dc.l {$82000000+NonHandledInterrupt}	; irq17
	dc.l {$82000000+NonHandledInterrupt}	; irq18
	dc.l {$82000000+NonHandledInterrupt}	; irq19
	dc.l {$82000000+NonHandledInterrupt}	; irq20
	dc.l {$82000000+NonHandledInterrupt}	; irq21
	dc.l {$82000000+NonHandledInterrupt}	; irq22
	dc.l {$82000000+NonHandledInterrupt}	; irq23
	dc.l {$82000000+NonHandledInterrupt}	; irq24
	dc.l {$82000000+NonHandledInterrupt}	; irq25
	dc.l {$82000000+NonHandledInterrupt}	; irq26
	dc.l {$82000000+NonHandledInterrupt}	; irq27
	dc.l {$82000000+NonHandledInterrupt}	; irq28
	dc.l {$82000000+NonHandledInterrupt}	; irq29

	end
