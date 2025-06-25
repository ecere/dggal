.PHONY: all clean realclean distclean dggal-sys dggal

DGGAL_ABSPATH := $(dir $(realpath $(firstword $(MAKEFILE_LIST))))../../

ifndef EC_SDK_SRC
EC_SDK_SRC := $(DGGAL_ABSPATH)../eC
endif

_CF_DIR = $(EC_SDK_SRC)/
include $(_CF_DIR)crossplatform.mk

# TARGETS

all: dggal-sys dggal

dggal-sys:
	+$(_MAKE) -f Makefile.dggal-sys

dggal: dggal-sys
	+$(_MAKE) -f Makefile.dggal

clean:
	+$(_MAKE) -f Makefile.dggal-sys clean
	+$(_MAKE) -f Makefile.dggal clean
	
realclean:
	+$(_MAKE) -f Makefile.dggal-sys realclean
	+$(_MAKE) -f Makefile.dggal realclean

distclean:
	+$(_MAKE) -f Makefile.dggal-sys distclean
	+$(_MAKE) -f Makefile.dggal distclean
