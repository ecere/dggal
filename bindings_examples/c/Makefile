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

DGGAL_ABSPATH := $(dir $(realpath $(firstword $(MAKEFILE_LIST))))../../../

ifndef ECERE_SDK_SRC
ECERE_SDK_SRC := $(DGGAL_ABSPATH)../ecere-sdk
endif

_CF_DIR = $(ECERE_SDK_SRC)/
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
	$(OBJ)eC$(O) \
	$(OBJ)ecere$(O) \
	$(OBJ)dggal$(O) \
	$(OBJ)info$(O)

OBJECTS = $(_OBJECTS)

SOURCES = \
	../../bindings/c/eC.c \
	../../bindings/c/ecere.c \
	../../bindings/c/dggal.c \
	info.c

RESOURCES =

ifdef USE_RESOURCES_EAR
RESOURCES_EAR =
else
RESOURCES_EAR = $(RESOURCES)
endif

LIBS += $(SHAREDLIB) $(EXECUTABLE) $(LINKOPT)

ifndef STATIC_LIBRARY_TARGET
LIBS += \
	$(call _L,ecere)
endif

PRJ_CFLAGS += \
	 $(if $(DEBUG), -g, -O2 -ffast-math) $(FPIC) -Wall -DREPOSITORY_VERSION="\"$(REPOSITORY_VER)\"" \
			 -I../../bindings/c

ECFLAGS += -module $(MODULE)
CECFLAGS += -cpp $(_CPP)

ifndef STATIC_LIBRARY_TARGET
OFLAGS += \
	 -L$(call quote_path,../../obj/release.$(PLATFORM)$(COMPILER_CONFIG))
endif

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
ifndef STATIC_LIBRARY_TARGET
	$(LD) $(OFLAGS) @$(OBJ)objects.lst $(LIBS) -o $(TARGET) $(INSTALLNAME) $(SONAME)
ifndef NOSTRIP
	$(STRIP) $(STRIPOPT) $(TARGET)
endif
else
ifdef WINDOWS_HOST
	$(AR) rcs $(TARGET) @$(OBJ)objects.lst $(LIBS)
else
	$(AR) rcs $(TARGET) $(OBJECTS) $(LIBS)
endif
endif
ifdef SHARED_LIBRARY_TARGET
ifdef LINUX_TARGET
ifdef LINUX_HOST
	$(if $(basename $(VER)),ln -sf $(LP)$(MODULE)$(SO)$(VER) $(OBJ)$(LP)$(MODULE)$(SO)$(basename $(VER)),)
	$(if $(VER),ln -sf $(LP)$(MODULE)$(SO)$(VER) $(OBJ)$(LP)$(MODULE)$(SO),)
endif
endif
endif

# SYMBOL RULES

# C OBJECT RULES

# OBJECT RULES

$(OBJ)eC$(O): ../../bindings/c/eC.c
	$(CC) $(CFLAGS) $(PRJ_CFLAGS) -c $(call quote_path,../../bindings/c/eC.c) -o $(call quote_path,$@)

$(OBJ)ecere$(O): ../../bindings/c/ecere.c
	$(CC) $(CFLAGS) $(PRJ_CFLAGS) -c $(call quote_path,../../bindings/c/ecere.c) -o $(call quote_path,$@)

$(OBJ)dggal$(O): ../../bindings/c/dggal.c
	$(CC) $(CFLAGS) $(PRJ_CFLAGS) -c $(call quote_path,../../bindings/c/dggal.c) -o $(call quote_path,$@)

$(OBJ)info$(O): info.c
	$(CC) $(CFLAGS) $(PRJ_CFLAGS) -c $(call quote_path,info.c) -o $(call quote_path,$@)

cleantarget:
	$(call rm,$(OBJ)objects.lst)
	$(call rm,$(TARGET))
ifdef SHARED_LIBRARY_TARGET
ifdef LINUX_TARGET
ifdef LINUX_HOST
	$(call rm,$(OBJ)$(LP)$(MODULE)$(SO)$(basename $(VER)))
	$(call rm,$(OBJ)$(LP)$(MODULE)$(SO))
endif
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
