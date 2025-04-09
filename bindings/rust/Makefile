ifneq ($(V),1)
.SILENT:
endif

MODULE := dggal

# CORE VARIABLES

VERSION := 0.0.1
CONFIG := release
ifndef COMPILER
COMPILER := default
endif

TARGET_TYPE = sharedlib

# INCLUDES

DGGAL_ABSPATH := $(dir $(realpath $(firstword $(MAKEFILE_LIST))))../../

ifndef EC_SDK_SRC
EC_SDK_SRC := $(DGGAL_ABSPATH)../eC
endif

_CF_DIR = $(EC_SDK_SRC)/
include $(_CF_DIR)crossplatform.mk
include $(_CF_DIR)default.cf

# POST-INCLUDES VARIABLES

OBJ = obj/$(PLATFORM)$(COMPILER_SUFFIX)

TARGET = $(OBJ)/lib$(MODULE).rlib

SOURCES = $(MODULE).rs

ifndef STATIC_LIBRARY_TARGET
LIBS += \
	$(call _L,$(MODULE))_c \
	$(call _L,ecrt_c) \
	$(call _L,ecrt)
endif

OFLAGS =

ifndef STATIC_LIBRARY_TARGET
OFLAGS += \
	 -Lnative=$(DGGAL_ABSPATH)obj/$(PLATFORM)$(COMPILER_SUFFIX)$(DEBUG_SUFFIX)/lib \
	 -Lnative=$(DGGAL_ABSPATH)obj/$(PLATFORM)$(COMPILER_SUFFIX)$(DEBUG_SUFFIX)/bin
endif

# TARGETS

.PHONY: all
all: objdir $(TARGET);

.PHONY: objdir
objdir:
	$(call mkdir,$(OBJ))

$(SYMBOLS): | objdir
$(OBJECTS): | objdir
$(TARGET): $(SOURCES) $(OBJECTS) | objdir
	rustc --edition 2021 --crate-type=lib $(OFLAGS) $(SOURCES) $(LIBS) -o $(TARGET)
	$(call mkdir,-p $(DGGAL_ABSPATH)$(HOST_SODESTDIR))
	$(call cp,$(TARGET),$(DGGAL_ABSPATH)$(HOST_SODESTDIR))

.PHONY: cleantarget
cleantarget:
	$(call rm,$(TARGET))

.PHONY: clean
clean: cleantarget

.PHONY: realclean
realclean: cleantarget
	$(call rmr,$(OBJ))

.PHONY: distclean
distclean:
	$(_MAKE) -f $(_CF_DIR)Cleanfile distclean distclean_all_subdirs
