;Copyright (C) 2003  Sridhar Ratnakumar <srid@srid.ca>
;Distributed under same license as rest of the program i.e. GPL v2 or later.
;See COPYING file for license text.

;See docs/asm-guide.txt before
;continuing

;Just debug this
;erroneos program ;-)

;A sample program
;illustrating debuging facilities
;"step-in", "step-over", "step-out"

;To see asm listing choose
;Assembler=>ShowListing menu

	jmp	start

;data
hai:	db 34,56
tmp:	equ 39h

;code
start:	mov a,h
	;push	b
	
;Try hitting any of 3 debugging buttons
;for understanding its mechanism
;Try setting breakpoints and hitting
;the "execute" button. (There are 2
;breakpoints set automatically when this
;file is opened!)

	call func1	;call func1
	
	pop b

	mov a,h
	call func2	;call func2
	call func3	;call func3
	mov m,a
	

	hlt

func1:	nop
	nop
	nop
	mvi c,56
	ret

func2:	nop
	nop
	mvi b,45h
	nop
	ret

func3:	nop
	push b
	nop
;@ (automatic break point)
	mvi d,34h
	call func2
	nop
;@
	pop b
	
	ret
