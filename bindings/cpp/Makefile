.PHONY: all clean realclean distclean dggal_cpp

DGGAL_ABSPATH := $(dir $(realpath $(firstword $(MAKEFILE_LIST))))../../

ifndef EC_SDK_SRC
EC_SDK_SRC := $(DGGAL_ABSPATH)../eC
endif

export _CF_DIR = $(EC_SDK_SRC)/
include $(_CF_DIR)crossplatform.mk

# TARGETS

all: dggal_cpp

dggal_cpp:
	+$(_MAKE) -f Makefile.dggal_cpp

clean:
	+$(_MAKE) -f Makefile.dggal_cpp clean
	
realclean:
	+$(_MAKE) -f Makefile.dggal_cpp realclean

distclean:
	+$(_MAKE) -f Makefile.dggal_cpp distclean
