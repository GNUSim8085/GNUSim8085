#!/bin/sh

aclocal -I m4

autoheader

autoconf

automake -a -c
