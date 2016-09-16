;Copyright (C) 2003  Sridhar Ratnakumar <srid@srid.ca>
;Distributed under same license as rest of the program i.e. GPL v2 or later.
;See COPYING file for license text.

;Program to convert BCD to binary (8-bit)


	lda bcd		;load the bcd number
	mov e,a
	ani 0f0h	;mask

	rlc		;shift left 4 times
	rlc
	rlc
	rlc

	mov b,a
	xra a		;clear a

	mvi c,0ah	;initialize counter

loop1:	add b
	dcr c
	jnz loop1

	mov b,a
	mov a,e
	ani 0fh
	add b
	sta bin
	hlt

bcd:	db 23h
bin:	db 00h
