ifneq ($(V),1)
.SILENT:
endif

# CORE VARIABLES

BINDING_MODULE := dggal
BINDING_NAME := dggal
MODULE := $(BINDING_NAME)_c_fn
VERSION := 0.0.6
CONFIG := release
# CONTAINS_CXX := defined
ifndef COMPILER
COMPILER := default
endif

TARGET_TYPE = sharedlib

# FLAGS

ifndef DEBIAN_PACKAGE
CFLAGS = -std=c++17 -fno-exceptions -fno-rtti -Wcomment
LDFLAGS =
endif
PRJ_CFLAGS =
CECFLAGS =
OFLAGS = -Wl,-nostdlib
LIBS =

ifdef DEBUG
NOSTRIP := y
endif

CONSOLE = -mwindows

# INCLUDES

DGGAL_ABSPATH := $(dir $(realpath $(firstword $(MAKEFILE_LIST))))../../

ifndef EC_SDK_SRC
EC_SDK_SRC := $(DGGAL_ABSPATH)../eC
endif

export _CF_DIR = $(EC_SDK_SRC)/

include $(_CF_DIR)crossplatform.mk
include $(_CF_DIR)default.cf

# POST-INCLUDES VARIABLES

OBJ = obj/$(BINDING_NAME).$(CONFIG).$(PLATFORM)$(COMPILER_SUFFIX)$(DEBUG_SUFFIX)/

RES =

ifdef LINUX_TARGET
TARGET = $(OBJ)$(LP)$(MODULE)$(SO).$(VERSION)
SONAME = -Wl,-soname,$(LP)$(MODULE)$(SO).0
else
TARGET = $(OBJ)$(LP)$(MODULE)$(SO)
SONAME =
endif

_OBJECTS = $(_DEP_OBJECTS) \
	$(OBJ)$(BINDING_NAME)$(O)

OBJECTS = $(_OBJECTS)

SOURCES = $(_DEP_SOURCES) \
	$(BINDING_NAME)_c.c

LIBS += $(SHAREDLIB) $(EXECUTABLE) $(LINKOPT)

ifndef STATIC_LIBRARY_TARGET
LIBS += \
   $(call _L,dggal_c) \
	$(call _L,ecrt_c) \
	$(call _L,ecrt)
endif

PRJ_CFLAGS += \
	 $(if $(DEBUG), -g, -O2 -ffast-math) $(FPIC) -w -DBINDINGS_SHARED -I. #-DECPRFX=eC_

#(SPECIFIC_FLAGS)#

PRJ_CFLAGS +=  -I../c -I$(EC_SDK_SRC)/bindings/c

ifndef STATIC_LIBRARY_TARGET
OFLAGS += \
	-L$(DGGAL_ABSPATH)obj/$(PLATFORM)$(COMPILER_SUFFIX)$(DEBUG_SUFFIX)/bin \
	-L$(DGGAL_ABSPATH)obj/$(PLATFORM)$(COMPILER_SUFFIX)$(DEBUG_SUFFIX)/lib \
	-L$(EC_SDK_SRC)/obj/$(PLATFORM)$(COMPILER_SUFFIX)$(DEBUG_SUFFIX)/bin \
	-L$(EC_SDK_SRC)/obj/$(PLATFORM)$(COMPILER_SUFFIX)$(DEBUG_SUFFIX)/lib

endif

# PLATFORM-SPECIFIC OPTIONS

ifdef WINDOWS_TARGET

ifndef STATIC_LIBRARY_TARGET

OFLAGS += -static-libgcc

endif

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
	$(LD) $(OFLAGS) $(_OBJECTS) $(LIBS) -o $(TARGET) $(INSTALLNAME)
ifndef NOSTRIP
	$(STRIP) $(STRIPOPT) $(TARGET)
endif
ifdef LINUX_TARGET
ifdef LINUX_HOST
	$(if $(basename $(basename $(VER))),ln -sf $(LP)$(MODULE)$(SO)$(VER) $(OBJ)$(LP)$(MODULE)$(SO)$(basename $(basename $(VER))),)
	$(if $(basename $(VER)),ln -sf $(LP)$(MODULE)$(SO)$(VER) $(OBJ)$(LP)$(MODULE)$(SO)$(basename $(VER)),)
	$(if $(VER),ln -sf $(LP)$(MODULE)$(SO)$(VER) $(OBJ)$(LP)$(MODULE)$(SO),)
endif
endif
	$(call cp,$(TARGET),$(DGGAL_ABSPATH)$(SODESTDIR))
ifdef LINUX_TARGET
ifdef LINUX_HOST
	$(if $(basename $(basename $(VER))),ln -sf $(LP)$(MODULE)$(SO)$(VER) $(DGGAL_ABSPATH)$(SODESTDIR)$(LP)$(MODULE)$(SO)$(basename $(basename $(VER))),)
	$(if $(basename $(VER)),ln -sf $(LP)$(MODULE)$(SO)$(VER) $(DGGAL_ABSPATH)$(SODESTDIR)$(LP)$(MODULE)$(SO)$(basename $(VER)),)
	$(if $(VER),ln -sf $(LP)$(MODULE)$(SO)$(VER) $(DGGAL_ABSPATH)$(SODESTDIR)$(LP)$(MODULE)$(SO),)
endif
endif

# OBJECT RULES

$(OBJ)$(BINDING_NAME)$(O): $(BINDING_NAME)_c.c
	$(CC) $(CFLAGS) $(PRJ_CFLAGS) -c $(call quote_path,$<) -o $(call quote_path,$@)

.PHONY: cleantarget
cleantarget:
	$(call rm,$(TARGET))
ifdef LINUX_TARGET
ifdef LINUX_HOST
	$(call rm,$(OBJ)$(LP)$(MODULE)$(SO)$(basename $(VER)))
	$(call rm,$(OBJ)$(LP)$(MODULE)$(SO))
endif
endif

.PHONY: clean
clean: cleantarget
	$(call rm,$(_OBJECTS))

.PHONY: realclean
realclean: cleantarget
	$(call rmr,$(OBJ))

.PHONY: wipeclean
wipeclean:
	$(call rmr,obj/)

.PHONY: distclean
distclean:
	$(_MAKE) -f $(_CF_DIR)Cleanfile distclean distclean_all_subdirs

$(MAKEFILE_LIST): ;
$(SOURCES): ;
$(RESOURCES): ;
