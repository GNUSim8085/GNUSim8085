;Copyright (C) 2003  Sridhar Ratnakumar <srid@srid.ca>
;Distributed under same license as rest of the program i.e. GPL v2 or later.
;See COPYING file for license text.


;Program to swap hex digits

	jmp start

;data


;code
start:	mvi a,91h
	mov b,a
	ani 0fh

	mov c,a
	mov a,b
	ani 0f0h

	rrc
	rrc
	rrc
	rrc

	mov d,a
	mov a,c
	
	rlc
	rlc
	rlc
	rlc

	add d

	hlt
