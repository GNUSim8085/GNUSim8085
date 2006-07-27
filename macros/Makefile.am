## Please update this variable if any new macros are created

GNOME2_MACROS=					\
	compiler-flags.m4 curses.m4 gnome-common.m4 gnome-cxx-check.m4 \
	gnome-gettext.m4 gnome-pthread-check.m4 gnome-x-checks.m4 \
	linger.m4 check-utmp.m4 gnome-pkgconfig.m4 gnome-platform.m4

EXTRA_DIST=$(GNOME2_MACROS) autogen.sh
MAINTAINERCLEANFILES=macros2.dep

@MAINT@macros2.dep: Makefile.am
@MAINT@	@echo '$$(top_srcdir)/aclocal.m4: $(GNOME2_MACROS:%=macros2/%)' > $@
