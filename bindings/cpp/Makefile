.PHONY: all clean realclean distclean ecrt_cpp dggal_cpp

DGGAL_ABSPATH := $(dir $(realpath $(firstword $(MAKEFILE_LIST))))../../

ifndef EC_SDK_SRC
EC_SDK_SRC := $(DGGAL_ABSPATH)../eC
endif

_CF_DIR = $(EC_SDK_SRC)/
include $(_CF_DIR)crossplatform.mk

# TARGETS

all: ecrt_cpp dggal_cpp

ecrt_cpp:
	+$(_MAKE) -f Makefile.ecrt_cpp

dggal_cpp:
	+$(_MAKE) -f Makefile.dggal_cpp

clean:
	+$(_MAKE) -f Makefile.ecrt_cpp clean
	+$(_MAKE) -f Makefile.dggal_cpp clean
	
realclean:
	+$(_MAKE) -f Makefile.ecrt_cpp realclean
	+$(_MAKE) -f Makefile.dggal_cpp realclean

distclean:
	+$(_MAKE) -f Makefile.ecrt_cpp distclean
	+$(_MAKE) -f Makefile.dggal_cpp distclean
