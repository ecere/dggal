.PHONY: all clean realclean distclean test dggal dgg

DGGAL_ABSPATH := $(dir $(realpath $(firstword $(MAKEFILE_LIST))))

ifndef EC_SDK_SRC
EC_SDK_SRC := $(DGGAL_ABSPATH)../eC
endif

_CF_DIR = $(EC_SDK_SRC)/
include $(_CF_DIR)crossplatform.mk

# TARGETS

all: dgg

dggal:
	+$(_MAKE) -f Makefile.dggal
	+$(_MAKE) -f Makefile.dggal.static

dgg: dggal
	+$(_MAKE) -f Makefile.dgg
	+$(_MAKE) -f Makefile.dgg.static

test: all
	+cd tests && $(_MAKE) test

clean:
	+$(_MAKE) -f Makefile.dgg clean
	+$(_MAKE) -f Makefile.dgg.static clean
	+$(_MAKE) -f Makefile.dggal clean
	+$(_MAKE) -f Makefile.dggal.static clean
	+cd tests && $(_MAKE) clean
	
realclean:
	+$(_MAKE) -f Makefile.dgg realclean
	+$(_MAKE) -f Makefile.dgg.static realclean
	+$(_MAKE) -f Makefile.dggal realclean
	+$(_MAKE) -f Makefile.dggal.static realclean
	+cd tests && $(_MAKE) realclean
	
distclean:
	+$(_MAKE) -f Makefile.dgg distclean
	+$(_MAKE) -f Makefile.dgg.static distclean
	+$(_MAKE) -f Makefile.dggal distclean
	+$(_MAKE) -f Makefile.dggal.static distclean
	+cd tests && $(_MAKE) distclean
