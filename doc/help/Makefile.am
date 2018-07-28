images = *.png
source_tarball_files = userguide.md

htmlhelpdir = $(htmldir)/help
htmlhelp_DATA =
if BUILD_HELP
htmlhelp_DATA += $(images) userguide.htm
endif

EXTRA_DIST = $(source_tarball_files) $(images)

DISTCLEANFILES = Makefile

all: userguide.htm

force-update-doc: clean userguide.htm

userguide.htm: $(source_tarball_files)
if BUILD_HELP
	$(markdown) -o $(srcdir)/userguide.htm $(srcdir)/userguide.md
else
	echo "Missing markdown." \
	"Rerun configure and check for 'checking whether documentation can be changed and compiled... yes'!"
endif

maintainer-clean: clean maintainer-clean-recursive

clean:
	-rm -f *.htm

.PHONY: force-update-doc clean

