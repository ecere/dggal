.PHONY: all clean realclean distclean test dggal dgg bindings c_bindings cpp_bindings py_bindings rust_bindings

DGGAL_ABSPATH := $(dir $(realpath $(firstword $(MAKEFILE_LIST))))

ifndef EC_SDK_SRC
EC_SDK_SRC := $(DGGAL_ABSPATH)../eC
endif

_CF_DIR = $(EC_SDK_SRC)/
include $(_CF_DIR)crossplatform.mk

ifdef WINDOWS_HOST
PYTHON := python
else
PYTHON := python3
endif

# TARGETS

all: dgg

dggal:
	+$(_MAKE) -f Makefile.dggal
# NOTE: Still building the library itself which will not need the .a libraries
#ifndef DISABLED_STATIC_BUILDS
	+$(_MAKE) -f Makefile.dggal.static
#endif

dgg: dggal
	+$(_MAKE) -f Makefile.dgg
ifndef DISABLED_STATIC_BUILDS
	+$(_MAKE) -f Makefile.dgg.static
endif

c_bindings:
	+cd bindings/c && $(_MAKE)
	+cd bindings/c_fn && $(_MAKE)
	+cd bindings/c_fn && $(_MAKE) -f Makefile.allinone

cpp_bindings: c_bindings
	+cd bindings/cpp && $(_MAKE)

py_bindings: c_bindings
	+cd bindings/py && $(PYTHON) build_dggal.py

rust_bindings: c_bindings
	+cd bindings/rust && $(_MAKE)

bindings: c_bindings cpp_bindings py_bindings rust_bindings

test: all
	+cd tests && $(_MAKE) test

clean:
	+$(_MAKE) -f Makefile.dgg clean
	+$(_MAKE) -f Makefile.dgg.static clean
	+$(_MAKE) -f Makefile.dggal clean
	+$(_MAKE) -f Makefile.dggal.static clean
	+cd bindings/c && $(_MAKE) clean
	+cd bindings/c_fn && $(_MAKE) clean
	+cd bindings/c_fn && $(_MAKE) -f Makefile.allinone clean
	+cd bindings/cpp && $(_MAKE) clean
	+cd bindings/py && rm -rf *.c *.o *.so *.dyld *.dll __pycache__ projects
	+cd bindings/rust && $(_MAKE) clean
	+cd tests && $(_MAKE) clean
	
realclean:
	+$(_MAKE) -f Makefile.dgg realclean
	+$(_MAKE) -f Makefile.dgg.static realclean
	+$(_MAKE) -f Makefile.dggal realclean
	+$(_MAKE) -f Makefile.dggal.static realclean
	+cd bindings/c && $(_MAKE) realclean
	+cd bindings/c_fn && $(_MAKE) realclean
	+cd bindings/c_fn && $(_MAKE) -f Makefile.allinone realclean
	+cd bindings/cpp && $(_MAKE) realclean
	+cd bindings/py && rm -rf *.c *.o *.so *.dyld *.dll __pycache__ projects
	+cd bindings/rust && $(_MAKE) realclean
	+cd tests && $(_MAKE) realclean
	
distclean:
	+$(_MAKE) -f Makefile.dgg distclean
	+$(_MAKE) -f Makefile.dgg.static distclean
	+$(_MAKE) -f Makefile.dggal distclean
	+$(_MAKE) -f Makefile.dggal.static distclean
	+cd bindings/c && $(_MAKE) distclean
	+cd bindings/c_fn && $(_MAKE) distclean
	+cd bindings/c_fn && $(_MAKE) -f Makefile.allinone distclean
	+cd bindings/cpp && $(_MAKE) distclean
	+cd bindings/py && rm -rf *.c *.o *.so *.dyld *.dll __pycache__ projects
	+cd bindings/rust && $(_MAKE) distclean
	+cd tests && $(_MAKE) distclean
