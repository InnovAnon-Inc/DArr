# Install the pkg-config file.
pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = src/libdarr-resize.pc src/libdarr.pc src/darr-test.pc

DISTCLEANFILES = $(pkgconfig_DATA)
EXTRA_DIST = src/libdarr-resize.pc.in src/libdarr.pc.in src/darr-test.pc.in

SUBDIRS = src
dist_doc_DATA = README.md
ACLOCAL_AMFLAGS = -I m4

