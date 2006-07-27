## Process this file with automake to produce Makefile.in
## Created by Anjuta - will be overwritten
## If you don't want it to overwrite it,
## 	Please disable it in the Anjuta project configuration

SUBDIRS = intl po macros src pixmaps doc

gnusim8085docdir = ${prefix}/doc/GNUSim8085
gnusim8085doc_DATA = \
	README\
	COPYING\
	AUTHORS\
	ChangeLog\
	INSTALL\
	NEWS\
	TODO\
	ABOUT-NLS

EXTRA_DIST = $(gnusim8085doc_DATA)

gnomemenudir = $(prefix)/@NO_PREFIX_PACKAGE_MENU_DIR@/Development
gnomemenu_DATA = GNUSim8085.desktop



# Copy all the spec files. Of cource, only one is actually used.
dist-hook:
	for specfile in *.spec; do \
		if test -f $$specfile; then \
			cp -p $$specfile $(distdir); \
		fi \
	done

