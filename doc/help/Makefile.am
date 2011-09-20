images = *.png
source_tarball_files = *.xml

htmlhelpdir = $(htmldir)/help
htmlhelp_DATA = $(images) $(PACKAGE).htm

EXTRA_DIST = $(source_tarball_files) $(images)

DISTCLEANFILES = Makefile

all: $(PACKAGE).htm

force-update-doc: clean $(PACKAGE).htm

$(PACKAGE).htm: $(source_tarball_files)
if BUILD_HELP
	$(xsltproc) -o $(srcdir)/$(PACKAGE).htm $(xsl_path) $(srcdir)/$(PACKAGE).xml
else
	echo "Missing xsltproc or xhtml style sheet." \
	"Rerun configure and check for 'checking whether documentation can be changed and compiled... yes'!"
endif

maintainer-clean: clean maintainer-clean-recursive

clean:
	-rm -f *.htm

.PHONY: force-update-doc clean

