## Process this file with automake to produce Makefile.in

SUBDIRS = po src pixmaps doc data

gnusim8085_docdir = $(docdir)
gnusim8085_doc_DATA = \
	README.md\
	AUTHORS\
	NEWS\
	TODO\
	ABOUT-NLS

EXTRA_DIST = config.rpath m4/ChangeLog  $(gnusim8085doc_DATA)

DISTCLEANFILES = *.exe *.gz

gnomemenudir = $(datadir)/applications
gnomemenu_DATA = GNUSim8085.desktop

# Copy all the spec files. Of cource, only one is actually used.
dist-hook:
	for specfile in *.spec; do \
		if test -f $$specfile; then \
			cp -p $$specfile $(distdir); \
		fi \
	done






ACLOCAL_AMFLAGS = -I m4
