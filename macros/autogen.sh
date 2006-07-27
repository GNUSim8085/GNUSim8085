#!/bin/sh
# Run this to generate all the initial makefiles, etc.

DIE=0

if [ -n "$GNOME2_DIR" ]; then
	ACLOCAL_FLAGS="-I $GNOME2_DIR/share/aclocal $ACLOCAL_FLAGS"
	LD_LIBRARY_PATH="$GNOME2_DIR/lib:$LD_LIBRARY_PATH"
	PATH="$GNOME2_DIR/bin:$PATH"
	export PATH
	export LD_LIBRARY_PATH
fi

(autoconf --version) < /dev/null > /dev/null 2>&1 || {
  echo
  echo "**Error**: You must have \`autoconf' installed to compile $PKG_NAME."
  echo "Download the appropriate package for your distribution,"
  echo "or get the source tarball at ftp://ftp.gnu.org/pub/gnu/"
  DIE=1
}

# Determine whether to use configure.in or configure.ac

if [ -f $srcdir/configure.ac ]; then
  CONFIGURE="$srcdir/configure.ac"
else
  CONFIGURE="$srcdir/configure.in"
fi

(grep "^AC_PROG_INTLTOOL" "$CONFIGURE" >/dev/null) && {
  (intltoolize --version) < /dev/null > /dev/null 2>&1 || {
    echo
    echo "**Error**: You must have \`intltoolize' installed to compile $PKG_NAME."
    echo "Get ftp://ftp.gnome.org/pub/GNOME/stable/sources/intltool/intltool-0.10.tar.gz"
    echo "(or a newer version if it is available)"
    DIE=1
  }
}

(grep "^AM_PROG_XML_I18N_TOOLS" "$CONFIGURE" >/dev/null) && {
  (xml-i18n-toolize --version) < /dev/null > /dev/null 2>&1 || {
    echo
    echo "**Error**: You must have \`xml-i18n-toolize' installed to compile $PKG_NAME."
    echo "Get ftp://ftp.gnome.org/pub/GNOME/stable/sources/xml-i18n-tools/xml-i18n-tools-0.9.tar.gz"
    echo "(or a newer version of xml-i18n-tools or intltool if it is available)"
    DIE=1
  }
}

(grep "^AM_PROG_LIBTOOL" "$CONFIGURE" >/dev/null) && {
  (libtool --version) < /dev/null > /dev/null 2>&1 || {
    echo
    echo "**Error**: You must have \`libtool' installed to compile $PKG_NAME."
    echo "Get ftp://ftp.gnu.org/pub/gnu/libtool-1.2d.tar.gz"
    echo "(or a newer version if it is available)"
    DIE=1
  }
}


# if grep "^AM_[A-Z0-9_]\{1,\}_GETTEXT" "$CONFIGURE" >/dev/null; then
#  if grep "sed.*POTFILES" "$CONFIGURE" >/dev/null; then
#    GETTEXTIZE=""
#  else
#    if grep "^AM_GLIB_GNU_GETTEXT" "$CONFIGURE" >/dev/null; then
#      GETTEXTIZE="glib-gettextize"
#      GETTEXTIZE_URL="ftp://ftp.gtk.org/pub/gtk/v1.3/glib-1.3.11.tar.gz"
#    else
#      GETTEXTIZE="gettextize"
#      GETTEXTIZE_URL="ftp://alpha.gnu.org/gnu/gettext-0.10.35.tar.gz"
#    fi

#    $GETTEXTIZE --version < /dev/null > /dev/null 2>&1
#    if test $? -ne 0; then
#      echo
#      echo "**Error**: You must have \`$GETTEXTIZE' installed to compile $PKG_NAME."
#      echo "Get $GETTEXTIZE_URL"
#      echo "(or a newer version if it is available)"
#      DIE=1
#    fi
#  fi
# fi

(automake --version) < /dev/null > /dev/null 2>&1 || {
  echo
  echo "**Error**: You must have \`automake' installed to compile $PKG_NAME."
  echo "Get ftp://ftp.gnu.org/pub/gnu/automake-1.3.tar.gz"
  echo "(or a newer version if it is available)"
  DIE=1
  NO_AUTOMAKE=yes
}


# if no automake, don't bother testing for aclocal
test -n "$NO_AUTOMAKE" || (aclocal --version) < /dev/null > /dev/null 2>&1 || {
  echo
  echo "**Error**: Missing \`aclocal'.  The version of \`automake'"
  echo "installed doesn't appear recent enough."
  echo "Get ftp://ftp.gnu.org/pub/gnu/automake-1.3.tar.gz"
  echo "(or a newer version if it is available)"
  DIE=1
}

if test "$DIE" -eq 1; then
  exit 1
fi

if test -z "$*"; then
  echo "**Warning**: I am going to run \`configure' with no arguments."
  echo "If you wish to pass any to it, please specify them on the"
  echo \`$0\'" command line."
  echo
fi

case $CC in
xlc )
  am_opt=--include-deps;;
esac

for coin in `find $srcdir -name configure.in -print -o -name configure.ac -print`
do 
  dr=`dirname $coin`
  bn=`basename $coin`
  if test -f $dr/NO-AUTO-GEN; then
    echo skipping $dr -- flagged as no auto-gen
  else
    echo processing $dr
    ( cd $dr

      aclocalinclude="$ACLOCAL_FLAGS"

#      if test "$GETTEXTIZE"; then
#	echo "Creating $dr/aclocal.m4 ..."
#	test -r $dr/aclocal.m4 || touch $dr/aclocal.m4
#	echo "Running $GETTEXTIZE...  Ignore non-fatal messages."
#	echo "no" | $GETTEXTIZE --force --copy
#	echo "Making $dr/aclocal.m4 writable ..."
#	test -r $dr/aclocal.m4 && chmod u+w $dr/aclocal.m4
#      fi

      if grep "^AM_GNU_GETTEXT" configure.in >/dev/null; then
	if grep "sed.*POTFILES" configure.in >/dev/null; then
	  : do nothing -- we still have an old unmodified configure.in
	else
	  echo "Creating $dr/aclocal.m4 ..."
	  test -r $dr/aclocal.m4 || touch $dr/aclocal.m4
	  echo "Running gettextize...  Ignore non-fatal messages."
	  ./setup-gettext
	  echo "Making $dr/aclocal.m4 writable ..."
	  test -r $dr/aclocal.m4 && chmod u+w $dr/aclocal.m4
        fi
      fi
      if grep "^AM_GNOME_GETTEXT" configure.in >/dev/null; then
	echo "Creating $dr/aclocal.m4 ..."
	test -r $dr/aclocal.m4 || touch $dr/aclocal.m4
	echo "Running gettextize...  Ignore non-fatal messages."
	./setup-gettext
	echo "Making $dr/aclocal.m4 writable ..."
	test -r $dr/aclocal.m4 && chmod u+w $dr/aclocal.m4
      fi

      if grep "^AC_PROG_INTLTOOL" $bn >/dev/null; then
        echo "Running intltoolize..."
	intltoolize --copy --force --automake
      fi
      if grep "^AM_PROG_XML_I18N_TOOLS" $bn >/dev/null; then
        echo "Running xml-i18n-toolize..."
	xml-i18n-toolize --copy --force --automake
      fi
      if grep "^AM_PROG_LIBTOOL" $bn >/dev/null; then
	if test -z "$NO_LIBTOOLIZE" ; then 
	  echo "Running libtoolize..."
	  libtoolize --force --copy
	fi
      fi

      echo "Running aclocal $aclocalinclude ..."
      aclocal $aclocalinclude || {
        echo
	echo "**Error**: \`aclocal' failed. Please fix the warnings"
        echo "(probably missing development files) and try again."
	DIE=1
      }

      if test "$DIE" -eq 1; then
        exit 1
      fi

      if grep "^AM_CONFIG_HEADER" $bn >/dev/null; then
	echo "Running autoheader..."
	autoheader || {
          echo
          echo "**Error**: \`autoheader' failed. Please fix the warnings"
	  echo "(probably missing development files) and try again."
	  DIE=1
      }

      if test "$DIE" -eq 1; then
        exit 1
      fi

      fi
      echo "Running automake --gnu $am_opt ..."
      automake --add-missing --gnu $am_opt || {
        echo
	echo "**Error**: \`automake' failed. Please fix the warnings"
	echo "(probably missing development files) and try again."
	DIE=1
      }

      if test "$DIE" -eq 1; then
        exit 1
      fi

      echo "Running autoconf ..."
      autoconf || {
        echo
	echo "**Error**: \`autoconf' failed. Please fix the warnings"
	echo "(probably missing development files) and try again."
	DIE=1
      }

      if test "$DIE" -eq 1; then
        exit 1
      fi
    )
  fi
done

conf_flags="--enable-maintainer-mode --enable-compile-warnings" #--enable-iso-c

if test x$NOCONFIGURE = x; then
  echo Running $srcdir/configure $conf_flags "$@" ...
  $srcdir/configure $conf_flags "$@" \
  && echo Now type \`make\' to compile $PKG_NAME || exit 1
else
  echo Skipping configure process.
fi
