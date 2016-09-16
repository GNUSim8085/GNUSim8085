GNUSim8085 Assembly Language Guide
==================================

:Date: 2003-10
:Version: 1.0
:Authors: Sridhar Ratnakumar <srid@srid.ca>


Introduction
------------

A basic assembly program consists of 4 parts.
    
    a. Machine operations (mnemonics)
    b. Pseudo operations (like preprocessor in C)
    c. Labels
    d. Comments
        
In addition, you have constants in an assembly program.  Unless otherwise
specified, a constant which is always numberic is in decimal form. If appended
with a character ``h`` it is assumed to be in hexadecimal form.  If a hex
constant starts with an alpha-char don't forget to include the number ``0`` in
the begining, since that will help the assembler to differentiate between a
label and a constant.
        
Labels
------

Labels when given to any particular instruction/data in a program, takes the
address of that instruction or data as its value.  But it has different meaning
when given to ``EQU`` directive.  Then it takes the operand of ``EQU`` as its
value. Labels must always be placed in the first column and must be followed by
an instruction (no empty line). Labels must be followed by a ``:`` (colon), to
differentiate it from other tokens.
    
Pseudo Ops
----------

There are only 3 directives currently available in our assembly language.
    
    a. ``DB`` - define byte ( 8 bits )
    b. ``DS`` - define size (no. of bytes)
    c. ``EQU`` - like minimalistic ``#define`` in C
        
``DB`` is used to define space for an array of values specified by comma
seperated list. And the label (if given to the begining of ``DB``) is assigned
the address of the first data item.  For example,

::

    var1: db 34, 56h, 87

.. note::

    Assuming that the assembler has currently incremented its ``PC`` to
    ``4200h``, ``var1=4200h``, ``var1+1=4201h``, ``var1+2=4202h``.  Note that
    ``56h`` is actually considered to be a hex constant.  In this example 3
    bytes are assigned.

``DS`` is used to define the specified number of bytes to be assigned and
initialize them to zero.  To access each byte you can use the ``+`` or ``-``
operator along with label.  For example,

::

    var2: ds 8

.. note:

    Now when you use ``var2`` in the program it refers to the first byte of
    these eight bytes.  To refer other bytes, say 3rd byte, you have to use
    ``var2+3`` instead of simply ``var2``.  Hope you understand!  This concept
    also applies to ``DB``! '-' is used to backrefer variables, i.e., to refer
    just previous variables in the program!

``EQU`` behaves similar to ``#define`` in C.  But it is simple.  It can be used
to give names only to numeric constants.  Nesting of ``EQU`` is not allowed.
You can use ``EQU`` only in operands for pseudo ops and mneumonics.  For
example,
       
::
 
    jmp start              ;jump to code skipping data
    ;data starts here
    port1: equ  9h
    data:  equ  7fh
    var1:  db   data, 0    ;like - 7fh, 0
    ;code starts here
    start: lxi  h, var1    ;load var1 address in HL pair for addressing
           mov  a, m       ;load contents of var1 in reg A (i.e. 7fh in A)
           out  port1      ;send contents of reg A to port 9h
           in   port1      ;read from port1 and store value in reg A
           sta  var1+1     ;store contents of reg A in memory location var+1 (next to 7fh!)
           hlt             ;halt execution

.. note::
		   
    As you can see ``EQU`` defined labels can be used to give descriptive names
    to constants.  You should use them frequently in your program in order to
    avoid magic numbers.

Mnemonics
---------

After all, I am using my spare time to do all these things.  Writing a BIG
manual on 8085 instructions seems to be redundant and time consuming.  You can
refer many available text books on 8085 programming for this. (TODO: tutor
weblink?) But don't get upset! There are example programs in the docs section,
which you can get used to! :-)

Comments
--------

Comments start with a semi-colon (``;``).  As you can see in the previous
example, comments can be given to any part of the program. Anything after ``;``
is ignored by the assembler, except to one important character sequence...YES
READ ON..


Auto breakpoints
----------------

As you get acquainted with the application, you can use breakpoints to debug
your program.  But for certain programs, you have to display something to the
user before continuing.  A perfect example for this is the N-Queens
problem. Here finding all the solutions for (say) 8 queens is time consuming (it
involves a total of 92 solutions).  In my system, it took almost 1 minute to
computer all the solutions.  But in that I can see only the last solution, since
solutions are overwritten by subsequent ones.  Now I can give a breakpoint at
the place where the program finds the next solution.  When the breakpoint is
reached, I can stop and see the solution (by examining the variables) and then
continue for the next solution.

But for this program, everytime you load it, you have to set the breakpoints.
This can be automated.  To set the breakpoint (when the program is loaded) at
line number `n`, you have to put a special comment at line ``n-1``.  And this
comment should start at first column.  The sequence is

::

    ;@

If ``;@`` is encountered, the editor will set breakpoint in the next line.  For
obvious reasons, you can't set a breakpoint at first line in your program. For
an example, look at the N-Queens program in the docs section (`nqueens.asm`).


Final notes
-----------
    
    * Don't forget to include the ``HLT`` instruction somewhere else in the
      program to terminate it, otherwise you will be fooled!
	
    * Constant addresses should be used with caution. ``LDA 2200h`` will be ``3a
      00 22`` in machine code . So the actual address is again ``2200h``!


Thats all for now folks!
http://www.gnusim8085.org/
