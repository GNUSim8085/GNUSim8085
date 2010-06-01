## Process this file with automake to produce Makefile.in

## Created by Anjuta - will be overwritten
## If you don't want it to overwrite it,
## 	Please disable it in the Anjuta project configuration

INCLUDES = \
        -I$(top_srcdir) \
        $(GTK_CFLAGS) \
        $(GTKSOURCEVIEW2_CFLAGS)

gnusim8085_CFLAGS =\
	 -DPACKAGE_DOC_DIR=\"$(docdir)\"\
	 -Wall\
	 -O1\
	 -g

if WIN32
gnusim8085_CFLAGS += \
	 -DLOCALEDIR=\"locale\"\
        -mms-bitfields  \
        -mwindows
else
	 -DLOCALEDIR=\"$(localedir)\"
endif

bin_PROGRAMS = gnusim8085

gnusim8085_SOURCES = \
	callbacks.c\
	interface.c\
	main.c\
	support.c\
	callbacks.h\
	interface.h\
	support.h\
	8085.c\
	8085-instructions.c\
	8085-memblock.c\
	asm-ds-symtable.c\
	asm-err-comm.c\
	asm-source.c\
	asm-token.c\
	asm-id.c\
	asm-gensym.c\
	asm-genobj.c\
	8085-asm.c\
	gui-app.c\
	gui-editor.c\
	8085-link.c\
	gui-view.c\
	bridge.c\
	gui-list-memory.c\
	gui-list-message.c\
	gui-list-data.c\
	asm-listing.c\
	gui-list-stack.c\
	8085-asm.h\
	8085-instructions.h\
	8085-link.h\
	8085-memblock.h\
	8085.h\
	asm-ds-limits.h\
	asm-ds-symtable.h\
	asm-err-comm.h\
	asm-genobj.h\
	asm-gensym.h\
	asm-id.h\
	asm-listing.h\
	asm-source.h\
	asm-token.h\
	bridge.h\
	gui-app.h\
	gui-editor.h\
	gui-list-data.h\
	gui-list-memory.h\
	gui-list-message.h\
	gui-list-stack.h\
	gui-view.h\
	gui-keypad.h\
	gui-keypad.c\
	gui-input-symbol.h\
	gui-input-symbol.c\
	asm-id-info.h\
	asm-id-info.c

gnusim8085_LDFLAGS = 

gnusim8085_LDADD =  \
        $(GTK_LIBS)\
        $(GTKSOURCEVIEW2_LIBS)

if USE_GIO
INCLUDES += \
        $(GIO_CFLAGS)

gnusim8085_SOURCES += \
	file-op-gio.h\
	file-op-gio.c

gnusim8085_LDADD +=  \
        $(GIO_LIBS)
else
gnusim8085_SOURCES += \
	file-op.h\
	file-op.c
endif

LDADD = @LIBINTL@
