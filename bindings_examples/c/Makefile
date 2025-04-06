.PHONY: all objdir cleantarget clean realclean distclean

# CORE VARIABLES

MODULE := info
VERSION :=
CONFIG := release
ifndef COMPILER
COMPILER := default
endif

TARGET_TYPE = executable

# FLAGS

ECFLAGS =
ifndef DEBIAN_PACKAGE
CFLAGS =
LDFLAGS =
endif
PRJ_CFLAGS =
CECFLAGS =
OFLAGS =
LIBS =

ifdef DEBUG
NOSTRIP := y
endif

CONSOLE = -mconsole

# INCLUDES

DGGAL_ABSPATH := $(dir $(realpath $(firstword $(MAKEFILE_LIST))))../../

ifndef EC_SDK_SRC
EC_SDK_SRC := $(DGGAL_ABSPATH)../eC
endif

_CF_DIR = $(EC_SDK_SRC)/
include $(_CF_DIR)crossplatform.mk
include $(_CF_DIR)default.cf
ifndef WINDOWS_HOST
export LD_LIBRARY_PATH := $(LD_LIBRARY_PATH):$(DGGAL_ABSPATH)obj/$(CONFIG).$(HOST_PLATFORM)$(COMPILER_SUFFIX)$(DEBUG_SUFFIX)/
else
export PATH := $(PATH);$(DGGAL_ABSPATH)obj/$(CONFIG).$(HOST_PLATFORM)$(COMPILER_SUFFIX)$(DEBUG_SUFFIX)/
endif

# POST-INCLUDES VARIABLES

OBJ = obj/$(CONFIG).$(PLATFORM)$(COMPILER_SUFFIX)$(DEBUG_SUFFIX)/

RES =

TARGET_NAME := info

TARGET = obj/$(CONFIG).$(PLATFORM)$(COMPILER_SUFFIX)$(DEBUG_SUFFIX)/$(TARGET_NAME)$(OUT)

_OBJECTS = \
	$(OBJ)ecrt$(O) \
	$(OBJ)dggal$(O) \
	$(OBJ)info$(O)

OBJECTS = $(_OBJECTS)

SOURCES = \
	../../bindings/c/ecrt.c \
	../../bindings/c/dggal.c \
	info.c

RESOURCES =

ifdef USE_RESOURCES_EAR
RESOURCES_EAR =
else
RESOURCES_EAR = $(RESOURCES)
endif

LIBS += $(SHAREDLIB) $(EXECUTABLE) $(LINKOPT)

LIBS += $(call _L,ecrt)

PRJ_CFLAGS += \
	 $(if $(DEBUG), -g, -O2 -ffast-math) $(FPIC) -Wall -DREPOSITORY_VERSION="\"$(REPOSITORY_VER)\"" \
			 -I../../bindings/c

ECFLAGS += -module $(MODULE)
CECFLAGS += -cpp $(_CPP)

OFLAGS += \
	 -L$(call quote_path,../../obj/release.$(PLATFORM)$(COMPILER_CONFIG)) \
	 -L$(call quote_path,../../obj/$(PLATFORM)$(COMPILER_CONFIG)/lib) \
	 -L$(call quote_path,$(EC_SDK_SRC)/obj/$(PLATFORM)$(COMPILER_CONFIG)/lib) \
	 -L$(call quote_path,../../obj/$(PLATFORM)$(COMPILER_CONFIG)/bin) \
	 -L$(call quote_path,$(EC_SDK_SRC)/obj/$(PLATFORM)$(COMPILER_CONFIG)/bin)

# TARGETS

all: objdir $(TARGET)

objdir:
	$(if $(wildcard $(OBJ)),,$(call mkdir,$(OBJ)))

$(SYMBOLS): | objdir
$(OBJECTS): | objdir
$(TARGET): $(SOURCES) $(RESOURCES_EAR) $(SYMBOLS) $(OBJECTS) | objdir
	@$(call rm,$(OBJ)objects.lst)
	@$(call touch,$(OBJ)objects.lst)
	$(call addtolistfile,$(_OBJECTS),$(OBJ)objects.lst)
	$(LD) $(OFLAGS) @$(OBJ)objects.lst $(LIBS) -o $(TARGET) $(INSTALLNAME) $(SONAME)
ifndef NOSTRIP
	$(STRIP) $(STRIPOPT) $(TARGET)
endif
ifdef LINUX_TARGET
ifdef LINUX_HOST
	$(if $(basename $(VER)),ln -sf $(LP)$(MODULE)$(SO)$(VER) $(OBJ)$(LP)$(MODULE)$(SO)$(basename $(VER)),)
	$(if $(VER),ln -sf $(LP)$(MODULE)$(SO)$(VER) $(OBJ)$(LP)$(MODULE)$(SO),)
endif
endif

# SYMBOL RULES

# C OBJECT RULES

# OBJECT RULES

$(OBJ)ecrt$(O): ../../bindings/c/ecrt.c
	$(CC) $(CFLAGS) $(PRJ_CFLAGS) -c $(call quote_path,../../bindings/c/ecrt.c) -o $(call quote_path,$@)

$(OBJ)dggal$(O): ../../bindings/c/dggal.c
	$(CC) $(CFLAGS) $(PRJ_CFLAGS) -c $(call quote_path,../../bindings/c/dggal.c) -o $(call quote_path,$@)

$(OBJ)info$(O): info.c
	$(CC) $(CFLAGS) $(PRJ_CFLAGS) -c $(call quote_path,info.c) -o $(call quote_path,$@)

cleantarget:
	$(call rm,$(OBJ)objects.lst)
	$(call rm,$(TARGET))
ifdef LINUX_TARGET
ifdef LINUX_HOST
	$(call rm,$(OBJ)$(LP)$(MODULE)$(SO)$(basename $(VER)))
	$(call rm,$(OBJ)$(LP)$(MODULE)$(SO))
endif
endif

clean: cleantarget
	$(call rm,$(_OBJECTS))

realclean: cleantarget
	$(call rmr,$(OBJ))

distclean: cleantarget
	$(call rmr,obj/)
	$(call rmr,.configs/)
	$(call rm,*.ews)
	$(call rm,*.Makefile)
