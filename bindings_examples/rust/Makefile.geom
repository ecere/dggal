ifneq ($(V),1)
.SILENT:
endif

MODULE := geom

# CORE VARIABLES

VERSION := 0.0.1
CONFIG := release
ifndef COMPILER
COMPILER := default
endif

TARGET_TYPE = executable

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

TARGET = $(OBJ)/$(MODULE)

SOURCES = geom.rs

OFLAGS =

OFLAGS += \
	-L$(DGGAL_ABSPATH)obj/$(PLATFORM)$(COMPILER_SUFFIX)$(DEBUG_SUFFIX)/lib \
	-L$(EC_SDK_SRC)/obj/$(PLATFORM)$(COMPILER_SUFFIX)$(DEBUG_SUFFIX)/lib \
	-L$(DGGAL_ABSPATH)obj/$(PLATFORM)$(COMPILER_SUFFIX)$(DEBUG_SUFFIX)/bin \
	-L$(EC_SDK_SRC)/obj/$(PLATFORM)$(COMPILER_SUFFIX)$(DEBUG_SUFFIX)/bin \
	-L$(DGGAL_ABSPATH)obj/static.$(PLATFORM)$(COMPILER_SUFFIX)$(DEBUG_SUFFIX)

# TARGETS

.PHONY: all
all: objdir $(TARGET);

.PHONY: objdir
objdir:
	$(call mkdir,$(OBJ))

$(SYMBOLS): | objdir
$(OBJECTS): | objdir
$(TARGET): $(SOURCES) $(OBJECTS) | objdir
	rustc --edition 2021 --cfg 'feature="geom_cmd"' $(OFLAGS) $(SOURCES) $(LIBS) -o $(TARGET)

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
