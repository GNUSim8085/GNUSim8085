; Just for test purposes it would be nice to automatically generate test array (haystack) in which to find a needle.
; Fill memory with 8 elements: 0, 1.. 7 and value of element corresponds to it's addresses.
    mvi b, 7 ; last index to fill untill
    mvi h, 0 ; set up HL as memory pointer
    mvi l, 0 ; set up HL as memory pointer
loop: inr l ; increase l by one
    mov m, l
    mov a, l
    cmp b
    jm  loop

; Binary search
; Alghorithm is about like this
;  low = 0
;  high = length(haystack) - 1
;  result = 0xFF
;  while high > low {
;    // average between high and low
;    var average = (low + high) >>> 1
;    // index of middle element
;    var middle = average + low
;    var element = haystack[middle]
;    if needle == element {
;      result = middle
;      exit
;    } else if needle > element {
;      low = middle + 1
;    } else {
;      high = middle - 1
;    }
;  }


; Initialization block
    mvi b, 0 ; low@b - index from which to start search i.e. 0
    mvi c, 7 ; high@c - index where to stop search. Our haystack has 8 elements so index of last elemnt is 7
    mvi d, 7 ; needle@d - value to find
    mvi l, 9 ; result will be stored to memory at address 9
    mvi m, 255 ; clear result, 255 i.e. 0xFF means not found

while: mov a, c
    cmp b ; high > low
    jm exit
    ; ((low + high) >>> 1) + low;
    add c ; a already have low@b so let's add high@c
    rar ; shift to right i.e. divide by 2 so we can get an average
    add b ; average + low
    mov e, a ; middle@e
    ; get element from memory
    mov l, e ; memory address@l is middle@e
    mov a, m ; get to element@a from memory@m by index@l
    cmp d ; is element@a > needle@d?
    jz equals ; jz - jump on zero
    jm less ; jm - jump on negative
    jp greater ; jp - jump on positive
equals: mvi l, 9 ; store middle@e to memory on 9
    mov m, e
    jmp exit
less: mov a, e
    inr a
    mov b, a
    jmp while
greater: mov a, e
    dcr a
    mov c, a
    jmp while
exit: hlt
