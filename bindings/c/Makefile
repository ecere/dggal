.PHONY: all clean realclean distclean ecrt_c dggal_c

DGGAL_ABSPATH := $(dir $(realpath $(firstword $(MAKEFILE_LIST))))../../

ifndef EC_SDK_SRC
EC_SDK_SRC := $(DGGAL_ABSPATH)../eC
endif

_CF_DIR = $(EC_SDK_SRC)/
include $(_CF_DIR)crossplatform.mk

# TARGETS

all: ecrt_c dggal_c

ecrt_c:
	+$(_MAKE) -f Makefile.ecrt_c

dggal_c:
	+$(_MAKE) -f Makefile.dggal_c

clean:
	+$(_MAKE) -f Makefile.ecrt_c clean
	+$(_MAKE) -f Makefile.dggal_c clean
	
realclean:
	+$(_MAKE) -f Makefile.ecrt_c realclean
	+$(_MAKE) -f Makefile.dggal_c realclean

distclean:
	+$(_MAKE) -f Makefile.ecrt_c distclean
	+$(_MAKE) -f Makefile.dggal_c distclean
