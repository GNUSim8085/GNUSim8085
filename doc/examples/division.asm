jmp start
;data
v_divisor: ds 1
v_dividend: ds 1
v_remainder: ds 1
v_quotient: ds 1
;code
;function divide performs division by adding 1 for each subtraction
;;; input: two bytes on stack (dividend in B, divisor in C)
;;; output: two bytes on stack (quotient and remainder)
;;; Note: "quotient" is integer result of "dividend" divided by "divisor".
divide: nop
	;reset register H to accumulate result
	mvi h,00h
	;save the return address
	pop d
	;get the input parameters in BC
	pop b
	;move the dividend to A
	mov a,b
	cmp c; end if the divisor is less than the dividend
	jc end_divide
	;; division loop is essentially dividend=dividend-divisor; quotient++
	subdivision: sub c; decrement the dividend (in A) by the divisor (in C)
		inr h; increment the value of the result (H) by one
		cmp c; is dividend still greater than or equal to the divisor?
		jnc subdivision; if so, loop to keep subtracting
	;; finish division
	end_divide: mov l,a;move the remainder into HL
		push h;save the result (H) & remainder (L) on the stack
		push d;restore the return address
	ret
start: nop
	;dividend = 7 (amount that is divided)
	mvi a,7
	sta v_dividend
	;divisor = 3 (amount to divide by)
	mvi a,3
	sta v_divisor
	;load the dividend, divisor to B,C registers
	lda v_dividend
	mov b,a
	lda v_divisor
	mov c,a
	;put register BC on the stack
	push b
	;call the division
	call divide
	;get the quotient and remainder of the division in BC
	pop b
	mov a,b
	sta v_quotient
	mov a,c
	sta v_remainder
hlt
