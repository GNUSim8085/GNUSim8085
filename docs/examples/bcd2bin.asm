
;Program to convert BCD to binary (8-bit)


	lda	bcd	;load the bcd number
	mov	e,a
	ani	0f0h	;mask

	rlc		;shift left 4 times
	rlc	
	rlc	
	rlc	

	mov	b,a
	xra	a	;clear a

	mvi	c,0ah	;initialize counter

loop1:	add	b
	dcr	c
	jnz	loop1

	mov	b,a
	mov	a,e
	ani	0fh
	add	b
	sta	bin
	hlt

bcd:	db	23h
bin:	db	00h
