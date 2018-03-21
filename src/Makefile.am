## Process this file with automake to produce Makefile.in

## Created by Anjuta

AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""$(localedir)"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir). \
	$(OPENSSL_CFLAGS)"\" \
	-DPACKAGE_DATA_DIR=\""$(pkgdatadir)"\"

AM_CFLAGS =\
	 -Wall\
	 -g

bin_PROGRAMS = quick_cryptor

quick_cryptor_SOURCES = \
	main.c \
	fileman.c \
	fileman.h \
	quickman.h \
	quickman.c \
	input.txt

quick_cryptor_LDFLAGS = 

quick_cryptor_LDADD =  \
	$(OPENSSL_LIBS)

