

;Program to swap hex digits

	jmp	start

;data


;code
start:	mvi	a,91h
	mov	b,a
	ani	0fh

	mov	c,a
	mov	a,b
	ani	0f0h

	rrc
	rrc
	rrc
	rrc

	mov	d,a
	mov	a,c
	
	rlc
	rlc
	rlc
	rlc

	add	d

	hlt
