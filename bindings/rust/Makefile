.PHONY: all clean realclean distclean ecrt-sys dggal-sys dggal

DGGAL_ABSPATH := $(dir $(realpath $(firstword $(MAKEFILE_LIST))))../../

ifndef EC_SDK_SRC
EC_SDK_SRC := $(DGGAL_ABSPATH)../eC
endif

_CF_DIR = $(EC_SDK_SRC)/
include $(_CF_DIR)crossplatform.mk

# TARGETS

all: ecrt-sys dggal-sys dggal

ecrt-sys:
	+$(_MAKE) -f Makefile.ecrt-sys

dggal-sys: ecrt-sys
	+$(_MAKE) -f Makefile.dggal-sys

dggal: dggal-sys
	+$(_MAKE) -f Makefile.dggal

clean:
	+$(_MAKE) -f Makefile.ecrt-sys clean
	+$(_MAKE) -f Makefile.dggal-sys clean
	+$(_MAKE) -f Makefile.dggal clean
	
realclean:
	+$(_MAKE) -f Makefile.ecrt-sys realclean
	+$(_MAKE) -f Makefile.dggal-sys realclean
	+$(_MAKE) -f Makefile.dggal realclean

distclean:
	+$(_MAKE) -f Makefile.ecrt-sys distclean
	+$(_MAKE) -f Makefile.dggal-sys distclean
	+$(_MAKE) -f Makefile.dggal distclean
