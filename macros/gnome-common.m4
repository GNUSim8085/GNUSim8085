# gnome-common.m4
# 

dnl GNOME_COMMON_INIT

AC_DEFUN([GNOME_COMMON_INIT],
[
	AC_CACHE_VAL(ac_cv_gnome_aclocal_dir,
	[ac_cv_gnome_aclocal_dir="$GNOME_COMMON_MACROS_DIR"])
	AC_CACHE_VAL(ac_cv_gnome_aclocal_flags,
	[ac_cv_gnome_aclocal_flags="$ACLOCAL_FLAGS"])
	GNOME_ACLOCAL_DIR="$ac_cv_gnome_aclocal_dir"
	GNOME_ACLOCAL_FLAGS="$ac_cv_gnome_aclocal_flags"
	AC_SUBST(GNOME_ACLOCAL_DIR)
	AC_SUBST(GNOME_ACLOCAL_FLAGS)

	ACLOCAL="$ACLOCAL $GNOME_ACLOCAL_FLAGS"

	AM_CONDITIONAL(INSIDE_GNOME_DOCU, false)
])

AC_DEFUN([GNOME_GTKDOC_CHECK],
[
	AC_CHECK_PROG(GTKDOC, gtkdoc-mkdb, true, false)
	AM_CONDITIONAL(HAVE_GTK_DOC, $GTKDOC)
	AC_SUBST(HAVE_GTK_DOC)

	dnl Let people disable the gtk-doc stuff.
	AC_ARG_ENABLE(gtk-doc, [  --enable-gtk-doc  Use gtk-doc to build documentation [default=auto]], enable_gtk_doc="$enableval", enable_gtk_doc=auto)

	if test x$enable_gtk_doc = xauto ; then
	  if test x$GTKDOC = xtrue ; then
	    enable_gtk_doc=yes
	  else
	    enable_gtk_doc=no
	  fi
	fi

	dnl NOTE: We need to use a separate automake conditional for this
	dnl       to make this work with the tarballs.
	AM_CONDITIONAL(ENABLE_GTK_DOC, test x$enable_gtk_doc = xyes)
])

AC_DEFUN([GNOME_DEBUG_CHECK],
[
	AC_ARG_ENABLE(debug, [  --enable-debug turn on debugging [default=no]], enable_debug="$enableval", enable_debug=no)

	if test x$enable_debug = xyes ; then
	  AC_DEFINE(GNOME_ENABLE_DEBUG)
	fi
])
