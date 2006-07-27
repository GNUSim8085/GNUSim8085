
;Test instructions

		jmp start

;data
v1:		db	33h
v2:		db	0

;code
start:		nop

		lxi	h, v1
		mvi	a, 20h
		;mvi	b, 33h
		add	m

		mvi	a, 33h
		mvi	b, 40h
		sub	b

		hlt
