stm8/

	#include "stm8s105c6.inc"
	#include "mapping.inc"

	segment 'rom'
main.l
	;BSET CLK_ECKR, #0 ;//HSEEN Разрешаем работу генератора с внешним кварцем (HSEEN)
  ;BSET CLK_SWCR, #1 ;//SWENРазрешаем переключение генераторов;
  ;mov CLK_SWR, $B4 ;//Выбираем clock от кварцевого генератора (HSE)
  mov CLK_CKDIVR, $07; //Делители частоты внутреннего и внешнего генератора на 1 - частота ядра максимальная
	
	mov IWDG_KR, $00
	
	; initialize SP
	ldw X,#stack_end
	ldw SP,X

	#ifdef RAM0	
	; clear RAM0
ram0_start.b EQU $ram0_segment_start
ram0_end.b EQU $ram0_segment_end
	ldw X,#ram0_start
clear_ram0.l
	clr (X)
	incw X
	cpw X,#ram0_end	
	jrule clear_ram0
	#endif

	#ifdef RAM1
	; clear RAM1
ram1_start.w EQU $ram1_segment_start
ram1_end.w EQU $ram1_segment_end	
	ldw X,#ram1_start
clear_ram1.l
	clr (X)
	incw X
	cpw X,#ram1_end	
	jrule clear_ram1
	#endif

	; clear stack
stack_start.w EQU $stack_segment_start
stack_end.w EQU $stack_segment_end
	ldw X,#stack_start
clear_stack.l
	clr (X)
	incw X
	cpw X,#stack_end	
	jrule clear_stack

  jra program ; обход подпрограмм
	
wait.w
loop.l
	decw Y
  JRNE loop
	ret
	 
step.w
  pushw Y
  BSET PD_ODR, #0 ; переключить светодиод 
	call wait   
	
	BRES PD_ODR, #0;
	popw Y
	call wait
	ret 
	 
program.l
	 mov PD_DDR,#255 ; установить режим работы порта
	 ld a, #1
start.l 
   ldw Y, #1
	 ldw $14, Y
	 ldw Y, #10000
	 ldw $12, Y
	 ldw Y, #1000
	 ldw $4, Y	
	 ldw Y, #70
	 ldw $2, Y
   ldw Y, $4
	 ldw $0, Y
	 
	 cp a, #1
	 jrne nota
	 ld a, #0
	 BSET PD_ODR, #1
	 jra longloop
nota.l	 
	 ld a, #1
	 BRES PD_ODR, #1
	 
longloop.l
   ldw x, $12
wait1.l
   ldw Y, $0
	 call step
   subw x, $0
	 cpw x, $0
	 jrugt wait1
	 
	 ldw Y, $0
	 decw Y
	 ldw $0, Y
	 cpw  Y, $2
	 jrne longloop
	 	 
   ld a, $14
waitm.l
   ldw X, $12
waiti.l
   ldw Y, $0
	 call step
	 decw x
	 jrne waiti
   dec a
	 jrne waitm
		nop 
longloop2.l
   ldw x, $12
wait2.l
   ldw Y, $0
	 call step
   subw x, $0
	 cpw x, $0
	 jrugt wait2
	 
	 ldw Y, $0
	 incw Y
	 ldw $0, Y
	 cpw Y, $4
	 jrne longloop2

  jra start

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
