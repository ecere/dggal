.PHONY: all clean realclean distclean dggal_c

DGGAL_ABSPATH := $(dir $(realpath $(firstword $(MAKEFILE_LIST))))../../

ifndef EC_SDK_SRC
EC_SDK_SRC := $(DGGAL_ABSPATH)../eC
endif

export _CF_DIR = $(EC_SDK_SRC)/
include $(_CF_DIR)crossplatform.mk

# TARGETS

all: dggal_c

dggal_c:
	+$(_MAKE) -f Makefile.dggal_c
	+$(_MAKE) -f Makefile.dggal_c.static

clean:
	+$(_MAKE) -f Makefile.dggal_c clean
	+$(_MAKE) -f Makefile.dggal_c.static clean
	
realclean:
	+$(_MAKE) -f Makefile.dggal_c realclean
	+$(_MAKE) -f Makefile.dggal_c.static realclean

distclean:
	+$(_MAKE) -f Makefile.dggal_c distclean
	+$(_MAKE) -f Makefile.dggal_c.static distclean
