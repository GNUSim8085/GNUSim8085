# GNUSim8085 (http://www.gnusim8085.org/)

GNUSim8085 is a graphical cross platform simulator plus assembler with
debugger for the Intel 8085 microprocessor. 

You have to enter assembly code in order to use the simulator.  The
assembly syntax is not too hard to learn. For more information see
"asm-guide.txt" in the doc/ directory.

Variable declarations are supported in assembler.  You have
a seperate Data view widget to show variable values during debugging.
And did I mention that?  Yes, you can also debug your code with the
builtin debugger!  For detailed stuff, you can generate program
listing which contains line by line of code with address and operand
information.

Contents of memory, IO ports can be modified during program execution.
Modification of registers, flag are being planned, but they are not
necessary always.

A lot of examples can be found in the docs directory! Try them.

Also don't forget to take a tutorial, when you will be asked during
the program startup.

## Dependencies

Build dependencies (Mandatory)
- GTK3 (>= 3.10.0) development package
- GtkSourceView3 (>= 3.10.0) development package

Build dependencies (Optional)
- gettext (>= 0.18)
- markdown (discount or similar package that provides markdown binary)

## Install from source
To install from source, run the following commands

```sh
$ ./autogen.sh # only required when configure script does not exist
$ ./configure
$ make
$ sudo make install
```

Or, see the INSTALL file.
