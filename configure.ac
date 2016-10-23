dnl Process this file with autoconf to produce a configure script.

AC_INIT([gnusim8085], [1.3.8], [https://github.com/GNUSim8085/GNUSim8085/issues])
AC_DEFINE([PACKAGE_URL],["http://www.gnusim8085.org/"],[Website URL])

AC_CANONICAL_HOST
AM_INIT_AUTOMAKE([1.11 silent-rules])
AM_SILENT_RULES([yes])
AC_CONFIG_SRCDIR([src/main.c])
AC_CONFIG_HEADERS([config.h])

AC_ISC_POSIX
AC_SUBST(CFLAGS,$CFLAGS)
AC_PROG_CC
AM_PROG_CC_STDC
AM_PROG_CC_C_O
AC_HEADER_STDC

dnl Checks for programs.
PKG_PROG_PKG_CONFIG

can_build_documentation=yes

dnl Checks for libraries.

dnl Checks for header files.
AC_CHECK_HEADERS([libintl.h stdlib.h string.h unistd.h])

dnl Checks for typedefs, structures, and compiler characteristics.

dnl Checks for library functions.
AC_CHECK_FUNCS([memset setlocale strtol strtoul])

dnl Checks for Additional stuffs.

case "${host}" in
        i[[3456789]]86-*-mingw32*) WIN32="yes" ;;
        *cygwin*) WIN32="yes" ;;
        *) WIN32="no" ;;
esac
AM_CONDITIONAL([WIN32], test "$WIN32" = "yes")

AC_PATH_PROG([xsltproc],[xsltproc])
if test -z "$xsltproc" ; then
	AC_MSG_WARN([xsltproc was not found. If you want to change and compile the documentation, \
please install libxslt (http://xmlsoft.org/XSLT/)])
	can_build_documentation=no
fi

xsl_path=/usr/share/xml/docbook/stylesheet/docbook-xsl/xhtml/docbook.xsl
if test "x$WIN32" = "xno"; then
AC_CHECK_FILE([$xsl_path],AC_SUBST(xsl_path), [AC_MSG_WARN([xhtml sytle sheet was not found. \
If you want to change and compile the documentation, please install docbook-xsl \
(http://docbook.sourceforge.net/)]) ; can_build_documentation=no])
fi

AC_MSG_CHECKING([whether documentation can be changed and compiled])
AC_MSG_RESULT($can_build_documentation)
AM_CONDITIONAL([BUILD_HELP], test "$can_build_documentation" = "yes")

gtk_api="gtk+-2.0"
gtksourceview_api="gtksourceview-2.0"
gtk_required="2.24.0"
gtksourceview_required="2.10.0"

AC_ARG_WITH([gtk3],
	AS_HELP_STRING([--with-gtk3],[Build with GTK3 (default: no, Windows build ignores this option)]),
	[with_gtk3=$withval],
	[with_gtk3=no])
if test "x$WIN32" != "xyes"; then
if test "x$with_gtk3" = "xyes"; then
gtk_api="gtk+-3.0"
gtksourceview_api="gtksourceview-3.0"
gtk_required="3.0.0"
gtksourceview_required="3.0.0"
fi
fi
AC_SUBST(GTK_VERSION_REQ, $gtk_required)

AC_ARG_ENABLE([gio],
	AS_HELP_STRING([--enable-gio],[Enable gio based file handling (default: enabled)]),
	[enable_gio=$enableval],
	[enable_gio=yes])
if test "x$enable_gio" != "xno"; then
	PKG_CHECK_MODULES(GIO, [gio-2.0],, [
    AC_MSG_ERROR(
    [
    Cannot find gio

        Please check the following:

         * Do you have the glib2 development package installed?
           (usually called glib2-devel for rpm based distros or
            libglib2.0-dev for deb based distros or similar)
           If
               ls -l /usr/lib/pkgconfig/gio-2.0.pc
           reports that the file does not exist, then you are missing
           the development package.

    ])])
fi
AM_CONDITIONAL(USE_GIO, test "x$enable_gio" = "xyes")

PKG_CHECK_MODULES(GTK, [$gtk_api >= $gtk_required],, [
    AC_MSG_ERROR(
    [
    Cannot find $gtk_api >= $gtk_required
    
        Please check the following:
    
         * Do you have the $gtk_api development package installed?
           (usually called $gtk_api-devel for rpm based distros or
            lib$gtk_api-dev for deb based distros or similar)
           If
               ls -l /usr/lib/pkgconfig/$gtk_api.pc
           reports that the file does not exist, then you are missing
           the development package.
    
    ])])

PKG_CHECK_MODULES(GTKSOURCEVIEW, [$gtksourceview_api >= $gtksourceview_required],, [
    AC_MSG_ERROR(
    [
    Cannot find $gtksourceview_api >= $gtksourceview_required
    
        Please check the following:
    
         * Do you have the $gtksourceview_api development package installed?
           (usually called $gtksourceview_api-devel for rpm based distros or
            lib$gtksourceview_api-dev for deb based distros or similar)
           If
               ls -l /usr/lib/pkgconfig/$gtksourceview_api.pc
           reports that the file does not exist, then you are missing
           the development package.
    
    ])])

AC_SUBST(GTK_LIBS)
AC_SUBST(GTK_CFLAGS)
AC_SUBST(GTKSOURCEVIEW_LIBS)
AC_SUBST(GTKSOURCEVIEW_CFLAGS)

dnl Languages which your application supports
AM_GNU_GETTEXT([external])
AM_GNU_GETTEXT_VERSION([0.18.1])

dnl Set PACKAGE SOURCE DIR in config.h.
packagesrcdir=`cd $srcdir && pwd`

dnl Set PACKAGE PREFIX
if test "x${prefix}" = "xNONE"; then
  packageprefix=${ac_default_prefix}
else
  packageprefix=${prefix}
fi

dnl Set PACKAGE DATA & DOC DIR
packagedatadir=share/${PACKAGE}
packagedocdir=share/doc/${PACKAGE}

dnl Set PACKAGE DIRs in config.h.
packagepixmapsdir=share/icons/hicolor/scalable/apps
packagehelpdir=${packagedocdir}/help
packagemenudir=share/gnome/apps

dnl Subst PACKAGE_DATA_DIR.
PACKAGE_DATA_DIR="${packageprefix}/${packagedatadir}"
AC_SUBST(PACKAGE_DATA_DIR)

dnl Subst PACKAGE_PIXMAPS_DIR.
PACKAGE_PIXMAPS_DIR="${packageprefix}/${packagepixmapsdir}"
AC_SUBST(PACKAGE_PIXMAPS_DIR)

dnl Subst PACKAGE_HELP_DIR.
PACKAGE_HELP_DIR="${packageprefix}/${packagehelpdir}"
AC_SUBST(PACKAGE_HELP_DIR)

dnl Subst PACKAGE_MENU_DIR.
PACKAGE_MENU_DIR="${packageprefix}/${packagemenudir}"
AC_SUBST(PACKAGE_MENU_DIR)

AC_DEFINE_UNQUOTED(PACKAGE_DATA_DIR, "${packageprefix}/${packagedatadir}", [Directory where data files are installed])
AC_DEFINE_UNQUOTED(PACKAGE_PIXMAPS_DIR, "${packageprefix}/${packagepixmapsdir}", [Directory where pixmaps files are installed])
AC_DEFINE_UNQUOTED(PACKAGE_HELP_DIR, "${packageprefix}/${packagehelpdir}", [Directory where help files are installed])
AC_DEFINE_UNQUOTED(PACKAGE_MENU_DIR, "${packageprefix}/${packagemenudir}", [Directory where menu files are installed])
dnl AC_DEFINE_UNQUOTED(PACKAGE_SOURCE_DIR, "${packagesrcdir}")

AC_OUTPUT([
Makefile
po/Makefile.in
src/Makefile
pixmaps/Makefile
doc/Makefile
doc/help/Makefile
data/Makefile
GNUSim8085.desktop
installer.nsi
])

