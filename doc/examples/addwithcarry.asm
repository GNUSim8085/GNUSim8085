;Copyright (C) 2003  Sridhar Ratnakumar <srid@srid.ca>
;Distributed under same license as rest of the program i.e. GPL v2 or later.
;See COPYING file for license text.

;Test add-with-carry instructions

	jmp start

;data


;code
start:	nop

	;Add with carry
	mvi a, 23h
	mvi b, 46h
	stc
	adc b	;6A
	;aci	46h

	;Sub with carry
	mvi a, 37h
	mvi b, 3fh
	stc
	sbb b

	;SBI
	mvi a, 37h
	stc
	sbi 25h

	hlt
