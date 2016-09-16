;Copyright (C) 2003  Sridhar Ratnakumar <srid@srid.ca>
;Distributed under same license as rest of the program i.e. GPL v2 or later.
;See COPYING file for license text.

;See docs/asm-guide.txt before
;continuing

;solution to N-Queens problem
;Sridhar <srid@srid.ca>

;TUTORIAL
;1. Click "execute" and expand the 
;   board variable to see solution
;2. Click "execute" again for next 
;   solution.
;3. For more information see 
;   doc/asm-guide.txt


	jmp start	;skip data

;data
nqueen:	equ 8	;INPUT - N

q_n:	db nqueen	;no of queens
sl_fd:	db 0		;solutions found
board:	ds nqueen	;max board size

;code
start:	nop
	
	mvi b,0
	call input	;input n
	call find_solution
	call output	;all sol found
	
	hlt		;halt program

;==============
;set board[B] = C
setb:	nop

	push h

	lxi h, board	;load addr
	mov a,l
	add b		;add index to l base
	mov l,a		;restore l
	jnc sb_c	;check for carry
	inr h
sb_c:	mov m,c		;store in memory
	
	pop h
	
	ret

;==============


;==============
;get board[D] in A
getb:	nop

	push h

	lxi h, board	;load addr
	mov a,l
	add d		;add index to l base
	mov l,a		;restore l
	jnc gb_c	;check for carry
	inr h
gb_c:	mov a,m		;save return value
	
	pop h
	
	ret

;==============

;==============
;threatens
; arg	B - x
; 	C - y
; loc	D - cnt
; 	E - tmp
;	H - threats (bool)
; ret	A - 0 if not threatens
threatens: nop
	
	push h

	mvi d,0
	mvi h,0
	;while cnt < x && h == 0
t_lp:	nop
	mov a,d
	cmp b
	jnc t_lpb
	mvi a,1
	cmp h
	jc t_lpb

	;get board[D] in A
	call getb
	
	cmp c
	jnz t_a
	mvi h,1
t_a:	nop

	mov a,b
	sub d
	mov e,a
	
	;? y == board[i]-tmp
	call getb
	sub e
	cmp c
	jz t_c
	
	;? y == board[i]+tmp
	call getb
	add e
	cmp c
	jnz t_b

t_c:	mvi h,1

t_b:	nop
	inr d

	;loop (while)
	jmp t_lp


t_lpb:	nop	

	mov a,h
	pop h
	ret

;==============

;==============
;Find Solution
; arg	B - pieces placed
; loc	C - counter

find_solution:	nop
	push b
	
	;init
	lxi h,q_n
	
	;for c=0 to N-1
	mvi c,0
fs_lp:	nop

	;check if threatens
	call threatens
	cpi 1
	jz fs_c

	;now not threatened
	call setb	;board[B] = C
	
	;if piecesplaced == n-1
	mov a,m
	dcr a
	cmp b
	jnz fs_a
	
	call ps		;print solution
	call ask	;ask user (pause)
	
	;sl_fd++
	push h
	lxi h,sl_fd
	inr m
	pop h

fs_a:	nop		;call recursively
	inr b
	call find_solution
	dcr b

	;jmp
fs_c:	inr c
	mov a,c
	cmp m
	jc fs_lp

	pop b
	ret
;==============


;==============
;Input n

input:	nop

	ret
;==============

;==============
;Print solution (g: board, q_n, sl_fd)

ps:	nop

	ret
;==============

;==============
;ask user whether to cont or not
;exit if 'n' or else just return

ask:	nop
;Execution should pause here
;Otherwise finding all solutions will
;take time - no one will wait for that!
;so we see the solutions one by one
;After execution breaks here, examine 
;the contents of variable "board" for
;sl_fd(th) solution!
;For N queens, solution is board[0->N-1]
;Resume execution by clicking "execute"
;button for next solution!
;The ";@" below is used to automatically
;set the breakpoint when opening this file

;@ - ";@" should be the first two chars of line before the line at which you wish to have the breakpoint.
	ret	;breakpoint is reached here. press "Execute" button to continue execution
;==============

;==============
;Output last message number of solutions

output:	nop

	ret
;EOF
